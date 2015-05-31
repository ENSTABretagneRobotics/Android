package com.example.gpssrv;

import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.location.GpsStatus;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.PowerManager;
import android.provider.Settings;
import android.view.Menu;

public class MainActivity extends Activity {

	private PowerManager powerManager;
	private PowerManager.WakeLock sdwl;
	private PowerManager.WakeLock pwl;

	private LocationManager locationManager;
	private GpsStatus.NmeaListener nmeaListener;
	private LocationListener locationListener;

	long utcMilliTime;
	double latitude;
	double longitude;
	double altitude;
	double speed;
	double bearing;

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

		// Get the Location Manager.
		locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);

		// Define a listener that responds to NMEA updates. NMEA is the
		// low-level GPS protocol.
		nmeaListener = new GpsStatus.NmeaListener() {
			public void onNmeaReceived(long timestamp, String nmea) {
				latestdata = nmea;
			}
		};

		// Register the NMEA listener.
		locationManager.addNmeaListener(nmeaListener);

		// Define a listener that responds to location updates.
		locationListener = new LocationListener() {
			public void onLocationChanged(Location location) {
				// Called when a new location is found by the location provider.
			}

			public void onStatusChanged(String provider, int status,
					Bundle extras) {
			}

			public void onProviderEnabled(String provider) {
			}

			public void onProviderDisabled(String provider) {
			}
		};

		// Register the listener with the Location Manager to receive location
		// updates from GPS.
		locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0,
				0, locationListener);

		previousdata = "";
		latestdata = "";

		new NetworkTask().execute("");
	}

	@Override
	public void onDestroy() {
		super.onDestroy(); // Always call the superclass method first.

		// Cannot destroy NetworkTask properly, however it should fail
		// immediately if started twice...

		locationManager.removeUpdates(locationListener);
		locationManager.removeNmeaListener(nmeaListener);
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

		// The activity is either being restarted or started for the first time
		// so this is where we should make sure that GPS is enabled.

		// Check if GPS is enabled.
		boolean gpsEnabled = locationManager
				.isProviderEnabled(LocationManager.GPS_PROVIDER);
		if (!gpsEnabled) {
			// Allow the user to enable manually GPS.
			enableLocationSettings();
		}
	}

	@Override
	protected void onStop() {
		super.onStop();  // Always call the superclass method first.
	}	

	class NetworkTask extends AsyncTask<String, Void, Integer> {

		protected Integer doInBackground(String... urls) {
			timeout = 5;
			srvport = 4001;					
			return LaunchSingleCliTCPSrv(srvport, null);
		}

		protected void onPostExecute(Integer param) {
			// TODO: check this.exception 
			// TODO: do something with the feed
		}
	}

	private void enableLocationSettings() {
		Intent settingsIntent = new Intent(
				Settings.ACTION_LOCATION_SOURCE_SETTINGS);
		startActivity(settingsIntent);
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
					if (latestdata.compareTo(previousdata) != 0) {					
						previousdata = latestdata;
						// Reset to blocking, so we can use blocking reads and writes.
						sc.configureBlocking(true);
						Socket s = sc.socket();

						s.setSoTimeout(DEFAULT_SOCK_TIMEOUT);

						s.getOutputStream().write(latestdata.getBytes("UTF-8"));
					}

					try {
						Thread.sleep(100);
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
