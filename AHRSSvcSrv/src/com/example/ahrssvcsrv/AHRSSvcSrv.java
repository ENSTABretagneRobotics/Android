package com.example.ahrssvcsrv;

import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.AsyncTask;
import android.os.IBinder;
import android.os.PowerManager;
import android.util.Log;

public class AHRSSvcSrv extends Service {
	private final static String TAG = "AHRSSvcSrv";
	
	protected boolean bStop = true;
	protected boolean bNetworkTaskRunning = false;

	private NotificationManager nm;

	private PowerManager powerManager;
	private PowerManager.WakeLock sdwl;
	private PowerManager.WakeLock pwl;
	
	private SensorManager sensorManager;
	private Sensor orientation;
	private SensorEventListener orientationListener;
	private Sensor gyroscope;
	private SensorEventListener gyroscopeListener;
	private Sensor accelerometer;
	private SensorEventListener accelerometerListener;

	long systemNanoTime;

	double azimuth;
	double pitch;
	double roll;

	double gyrx;
	double gyry;
	double gyrz;

	double accx;
	double accy;
	double accz;

	String previousdata;
	String latestdata;

//	public AHRSSvcSrv() {
//		super(TAG);
//		
//		Log.d(TAG, "AHRSSvcSrv()");		
//	}

	@Override
	public void onCreate() {
		super.onCreate();
		
		Log.d(TAG, "onCreate()");		
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		
		Log.d(TAG, "onDestroy()");
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		nm = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
		if (intent == null) return START_NOT_STICKY;
		String action = intent.getAction();
		if (action == null) return START_NOT_STICKY;
		Log.d(TAG, action);
		if ((bStop)&&(action.equals("START"))) {
			Log.d(TAG, "Starting AHRSSvcSrv service");			
			Notification notification = new Notification(R.drawable.ic_launcher, "AHRSSvcSrv service running",
					System.currentTimeMillis());
			notification.setLatestEventInfo(this, "AHRSSvcSrv Service", "Click to stop",
					PendingIntent.getService(this, 0, new Intent("STOP", null, this, this.getClass()), 0));
			notification.flags |= Notification.FLAG_ONGOING_EVENT;
			nm.notify(1, notification);	
			startForeground(1, notification);

			// Dim screen and prevent CPU sleep...
			powerManager = (PowerManager) getSystemService(Context.POWER_SERVICE);
			sdwl = powerManager.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, this
					.getApplication().toString());
			pwl = powerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, this
					.getApplication().toString());
			sdwl.acquire();
			pwl.acquire();

			// Initialize shared variables.
			previousdata = "";
			latestdata = "";

			// For 2.2 : TYPE_ORIENTATION (but deprecated and probably replaced by
			// TYPE_ROTATION_VECTOR), TYPE_GYROSCOPE declared but not fully
			// available until 2.3.

			sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
			orientation = sensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION);
			gyroscope = sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
			accelerometer = sensorManager
					.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);

			// Define a listener that responds to orientation updates.
			orientationListener = new SensorEventListener() {
				public void onSensorChanged(SensorEvent event) {
					systemNanoTime = System.nanoTime();
					azimuth = event.values[0];
					pitch = event.values[1];
					roll = event.values[2];

					// Angles here are not originally defined the same way as the Razor. 
					// Here, quick conversions are made, but contrary to the Razor, it is 
					// the roll that is expressed between -90 and 90 instead of the pitch.
					// To avoid this difference, we should turn the smartphone and consider 
					// almost directly the angles in the default coordinate system defined by 
					// Android, or compute more strictly by using a coordinate system change matrix
					// and identify the angles in the resulting new Euler matrix (Reuler1=P*Reuler0).
					
					DecimalFormatSymbols otherSymbols = new DecimalFormatSymbols();
					otherSymbols.setDecimalSeparator('.');
					otherSymbols.setGroupingSeparator(' ');
					DecimalFormat df = new DecimalFormat("0.00", otherSymbols);
					df.setGroupingUsed(false);
					latestdata = "#YPR="+df.format(fmod_360(azimuth))+","+df.format(-pitch)+","+df.format(-roll)+"\r\n";
					//Log.d(TAG, latestdata);
				}

				public void onAccuracyChanged(Sensor sensor, int accuracy) {
				}
			};

			sensorManager.registerListener(orientationListener, orientation,
					SensorManager.SENSOR_DELAY_NORMAL);

			// Define a listener that responds to gyroscope updates.
			gyroscopeListener = new SensorEventListener() {
				public void onSensorChanged(SensorEvent event) {
					systemNanoTime = System.nanoTime();
					gyrx = event.values[0];
					gyry = event.values[1];
					gyrz = event.values[2];
				}

				public void onAccuracyChanged(Sensor sensor, int accuracy) {
				}
			};

			sensorManager.registerListener(gyroscopeListener, gyroscope,
					SensorManager.SENSOR_DELAY_NORMAL);

			// Define a listener that responds to accelerometer updates.
			accelerometerListener = new SensorEventListener() {
				public void onSensorChanged(SensorEvent event) {
					systemNanoTime = System.nanoTime();
					accx = event.values[0];
					accy = event.values[1];
					accz = event.values[2];
				}

				public void onAccuracyChanged(Sensor sensor, int accuracy) {
				}
			};

			sensorManager.registerListener(accelerometerListener, accelerometer,
					SensorManager.SENSOR_DELAY_NORMAL);

			bStop = false;
			
			new NetworkTask().execute("");
			
			Log.d(TAG, "AHRSSvcSrv service started");
		} else if ((!bStop)&&(action.equals("STOP"))) {
			// User clicked the notification. Need to stop the service.
			Log.d(TAG, "Stopping AHRSSvcSrv service");

			// Should stop the NetworkTask.
			bStop = true;
			while (bNetworkTaskRunning)	{
				try {
					Thread.sleep(50);
				} catch (InterruptedException e) {
				}		
			}
			
			sensorManager.unregisterListener(accelerometerListener);
			sensorManager.unregisterListener(gyroscopeListener);
			sensorManager.unregisterListener(orientationListener);
			
			pwl.release();
			sdwl.release();
			
			nm.cancel(1);
			stopForeground(true);

			Log.d(TAG, "AHRSSvcSrv service stopped");
			stopSelf();
		}
		return START_NOT_STICKY;
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	class NetworkTask extends AsyncTask<String, Void, Integer> {

		protected void onPreExecute(Integer param) {
			bNetworkTaskRunning = true;
		}

		protected Integer doInBackground(String... urls) {
			timeout = 5;
			srvport = 4007;					
			return LaunchSingleCliTCPSrv(srvport, null);
		}

		protected void onPostExecute(Integer param) {	
			bNetworkTaskRunning = false;
		}
	}

	public double fmod_360(double theta) {
		return (((theta % (2 * 180)) + 3 * 180) % (2 * 180)) - 180;
	}

	public static final int EXIT_SUCCESS = 0;
	public static final int EXIT_FAILURE = 1;
	public static final int EXIT_TIMEOUT = 2;
	public static final int DEFAULT_SOCK_TIMEOUT = 10000;

	// Parameters.
	protected int timeout;
	protected int srvport;

	private ServerSocketChannel ssc;
	private SocketChannel sc;

	public int inittcpsrv(String address, int port, int maxnbcli, int timeout) {

		try {
			ssc = ServerSocketChannel.open();						
			ServerSocket ss = ssc.socket();
			ss.setSoTimeout(timeout); // Would be only for accept...?
			InetSocketAddress isa = new InetSocketAddress(address, port);
			ss.bind(isa, maxnbcli);
		} catch (Exception e) {
			System.out.println("inittcpsrv error : "+e);
			try { ssc.close(); } catch (Exception e2) { }
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}

	public int releasetcpsrv() {

		try {
			ssc.close();
		} catch (Exception e) {
			System.out.println("releasetcpsrv error : "+e);
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}

	public int disconnectclifromtcpsrv() {

		try {
			//sc.shutdownOutput();
			//sc.shutdownInput();
			sc.close();
		} catch (Exception e) {
			System.out.println("disconnectclifromtcpsrv error : "+e);
			try { sc.close(); } catch (Exception e2) { }
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}

	public int waitforclifortcpsrv(int timeout) {

		Selector selector = null;
		int num = 0;

		try {
			selector = Selector.open();

			// Set to non-blocking, so we can use select.
			ssc.configureBlocking(false);
			ssc.register(selector, SelectionKey.OP_ACCEPT);

			if (timeout != 0) {
				num = selector.select(timeout);
			}
			else {
				num = selector.selectNow();
			}

			selector.close();

			// If we don't have any activity
			if (num == 0) {
				return EXIT_TIMEOUT;
			}

			ServerSocket ss = ssc.socket();
			Socket s = ss.accept();
			sc = s.getChannel();

			System.out.println("Accepted connection from "+s);
		} catch (Exception e) {
			System.out.println("waitforclifortcpsrv error : "+e);
			try { selector.close(); } catch (Exception e2) { }
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}

	public int LaunchSingleCliTCPSrv(int port, Object param) {

		int iResult = EXIT_FAILURE;		

		if (inittcpsrv("0.0.0.0", port, 1, DEFAULT_SOCK_TIMEOUT) != EXIT_SUCCESS)
		{
			return EXIT_FAILURE;
		}

		while (!bStop)
		{			
			iResult = waitforclifortcpsrv(DEFAULT_SOCK_TIMEOUT);						
			switch (iResult)
			{
			case EXIT_SUCCESS:
				if (handlecli(param) != EXIT_SUCCESS) {
					System.out.println("LaunchSingleCliTCPSrv warning : Error while communicating with a client. ");
				}
				if (disconnectclifromtcpsrv() != EXIT_SUCCESS) {
					System.out.println("LaunchSingleCliTCPSrv warning : Error disconnecting a client. ");
				}
				break;
			case EXIT_TIMEOUT:
				break;
			default:
				releasetcpsrv();
				return EXIT_FAILURE;
			}
		}

		if (releasetcpsrv() != EXIT_SUCCESS)
		{
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}	

	public int handlecli(Object param) {

		Selector selector = null;
		int num = 0;

		while (!bStop) {
			try {					
				selector = Selector.open();

				// Set to non-blocking, so we can use select.
				sc.configureBlocking(false);
				sc.register(selector, SelectionKey.OP_WRITE);

				num = selector.select(timeout);

				selector.close();

				if (num != 0) {
					//if (latestdata.compareTo(previousdata) != 0) {					
						//previousdata = latestdata;
						// Reset to blocking, so we can use blocking reads and writes.
						sc.configureBlocking(true);
						Socket s = sc.socket();

						s.setSoTimeout(DEFAULT_SOCK_TIMEOUT);

						s.getOutputStream().write(latestdata.getBytes("UTF-8"));
					//}

					try {
						Thread.sleep(50);
					} catch (InterruptedException e) {
					}
				}			
			} catch (Exception e) {
				System.out.println("handlecli error : "+e+". ");
				try { selector.close(); } catch (Exception e2) { }
				return EXIT_FAILURE;
			}
		}

		try { selector.close(); } catch (Exception e2) { }
		return EXIT_SUCCESS;
	}	
}
