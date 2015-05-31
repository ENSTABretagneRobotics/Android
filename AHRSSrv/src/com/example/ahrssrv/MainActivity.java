package com.example.ahrssrv;

import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.text.DecimalFormat;

import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.PowerManager;
import android.view.Menu;

public class MainActivity extends Activity {

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

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		// Dim screen and prevent CPU sleep...
		powerManager = (PowerManager) getSystemService(Context.POWER_SERVICE);
		sdwl = powerManager.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, this
				.getApplication().toString());
		pwl = powerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, this
				.getApplication().toString());
		sdwl.acquire();
		pwl.acquire();

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
				
				DecimalFormat df = new DecimalFormat("0.00");
				latestdata = "#YPR="+df.format(fmod_360(azimuth))+","+df.format(-pitch)+","+df.format(-roll)+"\r\n";
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

		previousdata = "";
		latestdata = "";

		new NetworkTask().execute("");
	}

	@Override
	public void onDestroy() {
		super.onDestroy(); // Always call the superclass method first.

		// Cannot destroy NetworkTask properly, however it should fail
		// immediately if started twice...

		sensorManager.unregisterListener(accelerometerListener);
		sensorManager.unregisterListener(gyroscopeListener);
		sensorManager.unregisterListener(orientationListener);
		pwl.release();
		sdwl.release();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}

	@Override
	protected void onStart() {
		super.onStart();  // Always call the superclass method first.
	}

	@Override
	protected void onStop() {
		super.onStop();  // Always call the superclass method first.
	}	

	class NetworkTask extends AsyncTask<String, Void, Integer> {

		protected Integer doInBackground(String... urls) {
			timeout = 5;
			srvport = 4007;					
			return LaunchSingleCliTCPSrv(srvport, null);
		}

		protected void onPostExecute(Integer param) {
			// TODO: check this.exception 
			// TODO: do something with the feed
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

		for (;;)
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

		// Unreachable code.
		//if (releasetcpsrv() != EXIT_SUCCESS)
		//{
		//	return EXIT_FAILURE;
		//}

		//return EXIT_SUCCESS;
	}	

	public int handlecli(Object param) {

		Selector selector = null;
		int num = 0;

		for (;;) {
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

		// Unreachable code.
		//return EXIT_SUCCESS;
	}	
}
