package com.example.buggytriangle;

import ioio.lib.api.DigitalOutput;
import ioio.lib.api.PwmOutput;
import ioio.lib.api.exception.ConnectionLostException;
import ioio.lib.util.BaseIOIOLooper;
import ioio.lib.util.IOIOLooper;
import ioio.lib.util.android.IOIOActivity;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.Timer;
import java.util.TimerTask;

import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.location.GpsStatus;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.PowerManager;
import android.provider.Settings;
import android.view.Menu;
import android.widget.TextView;
import android.widget.ToggleButton;

/**
 * This is the main activity of the HelloIOIO example application.
 * 
 * It displays a toggle button on the screen, which enables control of the
 * on-board LED. This example shows a very simple usage of the IOIO, by using
 * the {@link IOIOActivity} class. For a more advanced use case, see the
 * HelloIOIOPower example.
 */
public class MainActivity extends IOIOActivity {

	private PowerManager powerManager;
	private PowerManager.WakeLock sdwl;
	private PowerManager.WakeLock pwl;

	private ToggleButton button_;
	int pw1 = 1500;
	int pw2 = 1500;
	int pw3 = 1500;

	private LocationManager locationManager;
	private GpsStatus.NmeaListener nmeaListener;
	private GpsStatus.Listener gpsStatusListener;
	private LocationListener locationListener;
	private SensorManager sensorManager;
	private Sensor orientation;
	private SensorEventListener orientationListener;

	Timer updateTimer;

	double latitude = 0;
	double longitude = 0;
	double azimuth = 0;

	double x = 0;
	double y = 0;
	double theta = 0;
	double u1 = 0;
	double u2 = 0;
	double thetaw = 0;
	double uw = 0;
	double xw = 0;
	double yw = 0;

	GPSPoint[] waypoints;
	GPSPoint gps0;
	int CurWP = 0;

	/**
	 * Called when the activity is first created. Here we normally initialize
	 * our GUI.
	 */
	@Override
	public void onCreate(Bundle savedInstanceState) {
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

		button_ = (ToggleButton) findViewById(R.id.button);

		// Get the Location Manager.
		locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);

		// Define a listener that responds to NMEA updates
		nmeaListener = new GpsStatus.NmeaListener() {
			public void onNmeaReceived(long timestamp, String nmea) {
				TextView nmeaTextView = (TextView) findViewById(R.id.nmeaTextView);
				nmeaTextView.setText(nmea);
				// Get the directory for the user's public download directory.
				File file = new File(
						Environment
								.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS),
						"nmea.txt");

				PrintWriter out = null;
				try {
					out = new PrintWriter(new BufferedWriter(new FileWriter(
							file, true)));
					out.println(nmea);
				} catch (Exception e) {

				} finally {
					if (out != null) {
						out.close();
					}
				}
			}
		};

		// Register the listener.
		locationManager.addNmeaListener(nmeaListener);

		// Define a listener
		gpsStatusListener = new GpsStatus.Listener() {
			public void onGpsStatusChanged(int event) {
			}
		};

		// Register the listener.
		locationManager.addGpsStatusListener(gpsStatusListener);

		// Define a listener that responds to location updates
		locationListener = new LocationListener() {
			public void onLocationChanged(Location location) {
				TextView utcTextView = (TextView) findViewById(R.id.utcMilliTimeTextView);
				utcTextView.setText(String.valueOf(location.getTime()));
				TextView latTextView = (TextView) findViewById(R.id.latitudeTextView);
				latTextView.setText(String.valueOf(location.getLatitude()));
				TextView longTextView = (TextView) findViewById(R.id.longitudeTextView);
				longTextView.setText(String.valueOf(location.getLongitude()));
				TextView altTextView = (TextView) findViewById(R.id.altitudeTextView);
				altTextView.setText(String.valueOf(location.getAltitude()));
				TextView speedTextView = (TextView) findViewById(R.id.speedTextView);
				speedTextView.setText(String.valueOf(location.getSpeed()));
				TextView bearingTextView = (TextView) findViewById(R.id.bearingTextView);
				bearingTextView.setText(String.valueOf(location.getBearing()));
				latitude = location.getLatitude();
				longitude = location.getLongitude();
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
		// updates
		locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0,
				0, locationListener);
		// locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER,
		// 0, 0, locationListener);

		// For 2.2 : TYPE_ACCELEROMETER, TYPE_MAGNETIC_FIELD, TYPE_ORIENTATION
		// (but deprecated and probably replaced by TYPE_ROTATION_VECTOR)

		sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
		// accelerometer =
		// sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		// magnetic_field =
		// sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
		orientation = sensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION);

		// Define a listener that responds to location updates
		orientationListener = new SensorEventListener() {
			public void onSensorChanged(SensorEvent event) {
				TextView azimuthTextView = (TextView) findViewById(R.id.azimuthTextView);
				azimuthTextView.setText(String.valueOf(event.values[0]));
				TextView pitchTextView = (TextView) findViewById(R.id.pitchTextView);
				pitchTextView.setText(String.valueOf(event.values[1]));
				TextView rollTextView = (TextView) findViewById(R.id.rollTextView);
				rollTextView.setText(String.valueOf(event.values[2]));
				azimuth = event.values[0];

				// Get the directory for the user's public download directory.
				File file = new File(
						Environment
								.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS),
						"orientation.txt");

				PrintWriter out = null;
				try {
					out = new PrintWriter(new BufferedWriter(new FileWriter(
							file, true)));
					out.println(System.nanoTime() + ";" + azimuth);
				} catch (Exception e) {

				} finally {
					if (out != null) {
						out.close();
					}
				}

				// Display also some last known values.
				TextView xTextView = (TextView) findViewById(R.id.xTextView);
				xTextView.setText(String.valueOf(x));
				TextView yTextView = (TextView) findViewById(R.id.yTextView);
				yTextView.setText(String.valueOf(y));
				TextView thetaTextView = (TextView) findViewById(R.id.thetaTextView);
				thetaTextView.setText(String.valueOf(theta * 180.0 / Math.PI));
				TextView thetawTextView = (TextView) findViewById(R.id.thetawTextView);
				thetawTextView
						.setText(String.valueOf(thetaw * 180.0 / Math.PI));
				TextView CurWPTextView = (TextView) findViewById(R.id.CurWPTextView);
				CurWPTextView.setText(String.valueOf(CurWP));
			}

			public void onAccuracyChanged(Sensor sensor, int accuracy) {
			}
		};

		sensorManager.registerListener(orientationListener, orientation,
				SensorManager.SENSOR_DELAY_UI);

		// GPS waypoints.
		waypoints = new GPSPoint[4];
		waypoints[0] = new GPSPoint(48.418514, -4.472238);
		waypoints[1] = new GPSPoint(48.418418, -4.472498);
		waypoints[2] = new GPSPoint(48.418612, -4.472510);
		waypoints[3] = new GPSPoint(48.418514, -4.472238);

		// GPS position of the reference coordinate system.
		gps0 = new GPSPoint(48.418514, -4.472238);

		updateTimer = new Timer();
		updateTimer.schedule(new updateTask(new Handler(), this), 0, 50);
	}

	@Override
	public void onDestroy() {
		super.onDestroy(); // Always call the superclass method first.

		updateTimer.cancel();
		updateTimer.purge();

		sensorManager.unregisterListener(orientationListener);
		locationManager.removeUpdates(locationListener);
		locationManager.removeGpsStatusListener(gpsStatusListener);
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
		super.onStart(); // Always call the superclass method first

		// The activity is either being restarted or started for the first time
		// so this is where we should make sure that GPS is enabled

		// Check if GPS is enabled.
		boolean gpsEnabled = locationManager
				.isProviderEnabled(LocationManager.GPS_PROVIDER);
		if (!gpsEnabled) {
			// new EnableGpsDialogFragment().show(getSupportFragmentManager(),
			// "enableGpsDialog");
			enableLocationSettings();
		}
	}

	@Override
	protected void onStop() {
		super.onStop(); // Always call the superclass method first
	}

	public void update() {

		// Get data from sensors .
		GPSPoint gps = new GPSPoint(latitude, longitude);
		RefCoordPoint pos = new RefCoordPoint();
		GPS2RefCoordSystem(gps0, gps, pos);
		x = pos.x;
		y = pos.y;
		theta = fmod_2PI(Math.PI / 2.0 - azimuth * Math.PI / 180.0);

		// Waypoints manager.
		RefCoordPoint posw = new RefCoordPoint();
		GPS2RefCoordSystem(gps0, waypoints[CurWP], posw);
		xw = posw.x;
		yw = posw.y;
		if (Math.sqrt(Math.pow(xw - x, 2) + Math.pow(yw - y, 2)) < 10) {
			CurWP++;
			if (CurWP >= waypoints.length) {
				CurWP = 0;
			}
		}

		// Waypoint controller.
		thetaw = Math.atan2(yw - y, xw - x); // Angle towards the waypoint.
		// uw = 1; // Maximum speed.
		uw = 0.5;

		// Heading controller.
		if (Math.cos(thetaw - theta) > 0.5) {
			u1 = Math.sin(thetaw - theta); // Proportional regulation w.r.t
			// desired heading.
		} else {
			u1 = sign(Math.sin(thetaw - theta), 0.0); // Bang-bang regulation if
			// far from desired
			// heading.
		}
		u2 = uw;

		// Set the actuators.
		if (Math.abs(pw1 - (1500 + (int) (u1 * 500))) > 50) {
			pw1 = 1500 + (int) (u1 * 500); // Front direction.
			pw2 = 1500; // Rear direction.
		}
		pw3 = 1500 + (int) (u2 * 500); // Motors.
	}

	/**
	 * This is the thread on which all the IOIO activity happens. It will be run
	 * every time the application is resumed and aborted when it is paused. The
	 * method setup() will be called right after a connection with the IOIO has
	 * been established (which might happen several times!). Then, loop() will
	 * be called repetitively until the IOIO gets disconnected.
	 */
	class Looper extends BaseIOIOLooper {
		/** The on-board LED. */
		private DigitalOutput led_;
		private PwmOutput pwm1_;
		private PwmOutput pwm2_;
		private PwmOutput pwm3_;

		/**
		 * Called every time a connection with IOIO has been established.
		 * Typically used to open pins.
		 * 
		 * @throws ConnectionLostException
		 *             When IOIO connection is lost.
		 * 
		 * @see ioio.lib.util.AbstractIOIOActivity.IOIOThread#setup()
		 */
		@Override
		protected void setup() throws ConnectionLostException {
			led_ = ioio_.openDigitalOutput(0, true);
			pwm1_ = ioio_.openPwmOutput(12, 50);
			pwm2_ = ioio_.openPwmOutput(13, 50);
			pwm3_ = ioio_.openPwmOutput(14, 50);
		}

		/**
		 * Called repetitively while the IOIO is connected.
		 * 
		 * @throws ConnectionLostException
		 *             When IOIO connection is lost.
		 * 
		 * @see ioio.lib.util.AbstractIOIOActivity.IOIOThread#loop()
		 */
		@Override
		public void loop() throws ConnectionLostException {
			led_.write(!button_.isChecked());
			pwm1_.setPulseWidth(pw1);
			pwm2_.setPulseWidth(pw2);
			pwm3_.setPulseWidth(pw3);

			try {
				Thread.sleep(50);
			} catch (InterruptedException e) {
			}
		}
	}

	/**
	 * A method to create our IOIO thread.
	 * 
	 * @see ioio.lib.util.AbstractIOIOActivity#createIOIOThread()
	 */
	@Override
	protected IOIOLooper createIOIOLooper() {
		return new Looper();
	}

	private class updateTask extends TimerTask {
		Handler handler;
		MainActivity ref;

		public updateTask(Handler handler, MainActivity ref) {
			super();
			this.handler = handler;
			this.ref = ref;
		}

		@Override
		public void run() {
			handler.post(new Runnable() {
				@Override
				public void run() {
					ref.update();
				}
			});
		}
	}

	// Method to launch Settings
	private void enableLocationSettings() {
		Intent settingsIntent = new Intent(
				Settings.ACTION_LOCATION_SOURCE_SETTINGS);
		startActivity(settingsIntent);
	}

	public double sign(double x, double epsilon) {
		if (x >= epsilon)
			return 1;
		else if (x <= -epsilon)
			return -1;
		else
			return x / epsilon;
	}

	public double fmod_2PI(double theta) {
		return (((theta % (2 * Math.PI)) + 3 * Math.PI) % (2 * Math.PI))
				- Math.PI;
	}

	public double fmod_360(double theta) {
		return (((theta % (2 * 180)) + 3 * 180) % (2 * 180)) - 180;
	}

	public class RefCoordPoint {
		public double x;
		public double y;

		public RefCoordPoint() {
			this.x = 0;
			this.y = 0;
		}

		public RefCoordPoint(double x, double y) {
			this.x = x;
			this.y = y;
		}
	}

	public class GPSPoint {
		public double latitude;
		public double longitude;

		public GPSPoint() {
			this.latitude = 0;
			this.longitude = 0;
		}

		public GPSPoint(double latitude, double longitude) {
			this.latitude = latitude;
			this.longitude = longitude;
		}
	}

	public static final double EARTH_RADIUS = 6371000.0;

	private void GPS2RefCoordSystem(GPSPoint gps0, GPSPoint gps,
			RefCoordPoint pos) {
		pos.x = (Math.PI / 180.0) * EARTH_RADIUS
				* (gps.longitude - gps0.longitude)
				* Math.cos((Math.PI / 180.0) * gps.latitude);
		pos.y = (Math.PI / 180.0) * EARTH_RADIUS
				* (gps.latitude - gps0.latitude);
	}

	private void RefCoordSystem2GPS(GPSPoint gps0, GPSPoint gps,
			RefCoordPoint pos) {
		gps.latitude = (pos.y / (double) EARTH_RADIUS) * (180.0 / Math.PI)
				+ gps0.latitude;
		if ((gps.latitude == 90.0) || (gps.latitude == -90.0)) {
			gps.longitude = 0;
		} else {
			gps.longitude = (pos.x / (double) EARTH_RADIUS) * (180.0 / Math.PI)
					/ Math.cos((Math.PI / 180.0) * (gps.latitude))
					+ gps0.longitude;
		}
	}

	private void LineGPS2RefCoordSystem(GPSPoint gps0, GPSPoint gpsa,
			GPSPoint gpsb, RefCoordPoint posa, RefCoordPoint posb) {
		GPS2RefCoordSystem(gps0, gpsa, posa);
		GPS2RefCoordSystem(gps0, gpsb, posb);
	}

//	/**
//	 * Dialog to prompt users to enable GPS on the device.
//	 */
//	public class EnableGpsDialogFragment extends DialogFragment {
//
//		@Override
//		public Dialog onCreateDialog(Bundle savedInstanceState) {
//			return new AlertDialog.Builder(getActivity())
//					.setTitle("Check GPS status")
//					.setMessage("Check GPS status")
//					.setPositiveButton("Check GPS status",
//							new DialogInterface.OnClickListener() {
//								public void onClick(DialogInterface dialog,
//										int which) {
//									enableLocationSettings();
//								}
//							}).create();
//		}
//	}
}
