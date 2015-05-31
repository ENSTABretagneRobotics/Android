package com.example.aslogger;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;

import android.app.Activity;
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
import android.os.PowerManager;
import android.provider.Settings;
import android.view.Menu;
import android.widget.TextView;

public class MainActivity extends Activity {

	private PowerManager powerManager;
	private PowerManager.WakeLock sdwl;
	private PowerManager.WakeLock pwl;

	private LocationManager locationManager;
	private GpsStatus.NmeaListener nmeaListener;
	private LocationListener locationListener;
	private SensorManager sensorManager;
	private Sensor orientation;
	private SensorEventListener orientationListener;
	private Sensor gyroscope;
	private SensorEventListener gyroscopeListener;
	private Sensor accelerometer;
	private SensorEventListener accelerometerListener;

	long systemNanoTime;

	long utcMilliTime;
	double latitude;
	double longitude;
	double altitude;
	double speed;
	double bearing;

	double azimuth;
	double pitch;
	double roll;

	double gyrx;
	double gyry;
	double gyrz;

	double accx;
	double accy;
	double accz;

	TextView systemNanoTimeTextView;

	TextView utcMilliTimeTextView;
	TextView latitudeTextView;
	TextView longitudeTextView;
	TextView altitudeTextView;
	TextView speedTextView;
	TextView bearingTextView;

	TextView azimuthTextView;
	TextView pitchTextView;
	TextView rollTextView;

	TextView gyrxTextView;
	TextView gyryTextView;
	TextView gyrzTextView;

	TextView accxTextView;
	TextView accyTextView;
	TextView acczTextView;

	TextView nmeaTextView;

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

		systemNanoTimeTextView = (TextView) findViewById(R.id.systemNanoTimeTextView);

		utcMilliTimeTextView = (TextView) findViewById(R.id.utcMilliTimeTextView);
		latitudeTextView = (TextView) findViewById(R.id.latitudeTextView);
		longitudeTextView = (TextView) findViewById(R.id.longitudeTextView);
		altitudeTextView = (TextView) findViewById(R.id.altitudeTextView);
		speedTextView = (TextView) findViewById(R.id.speedTextView);
		bearingTextView = (TextView) findViewById(R.id.bearingTextView);

		azimuthTextView = (TextView) findViewById(R.id.azimuthTextView);
		pitchTextView = (TextView) findViewById(R.id.pitchTextView);
		rollTextView = (TextView) findViewById(R.id.rollTextView);

		gyrxTextView = (TextView) findViewById(R.id.gyrxTextView);
		gyryTextView = (TextView) findViewById(R.id.gyryTextView);
		gyrzTextView = (TextView) findViewById(R.id.gyrzTextView);

		accxTextView = (TextView) findViewById(R.id.accxTextView);
		accyTextView = (TextView) findViewById(R.id.accyTextView);
		acczTextView = (TextView) findViewById(R.id.acczTextView);

		nmeaTextView = (TextView) findViewById(R.id.nmeaTextView);

		// Set up file header.

		// Get the directory for the user's public download directory.
		File file = new File(
				Environment
						.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS),
				"log.txt");
		PrintWriter out = null;
		try {
			out = new PrintWriter(
					new BufferedWriter(new FileWriter(file, true)));
			out.println("systemNanoTime (in ns);utcMilliTime (in ms);latitude (in deg);longitude (in deg);altitude (in m);speed (in m/s);bearing (in deg);azimuth (in deg);pitch (in deg);roll (in deg);gyrx (in rad/s);gyry (in rad/s);gyrz (in rad/s);accx (in m/s2);accy (in m/s2);accz (in m/s2);");
		} catch (Exception e) {

		} finally {
			if (out != null) {
				out.close();
			}
		}
		 
		// Get the Location Manager.
		locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);

		// Define a listener that responds to NMEA updates. NMEA is the
		// low-level GPS protocol.
		nmeaListener = new GpsStatus.NmeaListener() {
			public void onNmeaReceived(long timestamp, String nmea) {

				// Specific NMEA raw data file.

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
				nmeaTextView.setText(nmea);
			}
		};

		// Register the NMEA listener.
		locationManager.addNmeaListener(nmeaListener);

		// Define a listener that responds to location updates.
		locationListener = new LocationListener() {
			public void onLocationChanged(Location location) {
				// Called when a new location is found by the location provider.
				systemNanoTime = System.nanoTime();
				utcMilliTime = location.getTime();
				latitude = location.getLatitude();
				longitude = location.getLongitude();
				altitude = location.getAltitude();
				speed = location.getSpeed();
				bearing = location.getBearing();
				saveToFile();
				systemNanoTimeTextView.setText(String.valueOf(systemNanoTime));
				utcMilliTimeTextView.setText(String.valueOf(utcMilliTime));
				latitudeTextView.setText(String.valueOf(latitude));
				longitudeTextView.setText(String.valueOf(longitude));
				altitudeTextView.setText(String.valueOf(altitude));
				speedTextView.setText(String.valueOf(speed));
				bearingTextView.setText(String.valueOf(bearing));
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
				saveToFile();
				systemNanoTimeTextView.setText(String.valueOf(systemNanoTime));
				azimuthTextView.setText(String.valueOf(azimuth));
				pitchTextView.setText(String.valueOf(pitch));
				rollTextView.setText(String.valueOf(roll));
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
				saveToFile();
				systemNanoTimeTextView.setText(String.valueOf(systemNanoTime));
				gyrxTextView.setText(String.valueOf(gyrx));
				gyryTextView.setText(String.valueOf(gyry));
				gyrzTextView.setText(String.valueOf(gyrz));
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
				saveToFile();
				systemNanoTimeTextView.setText(String.valueOf(systemNanoTime));
				accxTextView.setText(String.valueOf(accx));
				accyTextView.setText(String.valueOf(accy));
				acczTextView.setText(String.valueOf(accz));
			}

			public void onAccuracyChanged(Sensor sensor, int accuracy) {
			}
		};

		sensorManager.registerListener(accelerometerListener, accelerometer,
				SensorManager.SENSOR_DELAY_NORMAL);
	}

	@Override
	public void onDestroy() {
		super.onDestroy(); // Always call the superclass method first.

		sensorManager.unregisterListener(accelerometerListener);
		sensorManager.unregisterListener(gyroscopeListener);
		sensorManager.unregisterListener(orientationListener);
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
		super.onStart(); // Always call the superclass method first.

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
		super.onStop(); // Always call the superclass method first.
	}

	private void saveToFile() {
		// Get the directory for the user's public download directory.
		File file = new File(
				Environment
						.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS),
				"log.txt");
		PrintWriter out = null;
		try {
			out = new PrintWriter(
					new BufferedWriter(new FileWriter(file, true)));
			out.println(systemNanoTime + ";" + utcMilliTime + ";" + latitude
					+ ";" + longitude + ";" + altitude + ";" + speed + ";"
					+ bearing + ";" + azimuth + ";" + pitch + ";" + roll + ";"
					+ gyrx + ";" + gyry + ";" + gyrz + ";" + accx + ";" + accy
					+ ";" + accz + ";");
		} catch (Exception e) {

		} finally {
			if (out != null) {
				out.close();
			}
		}
	}

	private void enableLocationSettings() {
		Intent settingsIntent = new Intent(
				Settings.ACTION_LOCATION_SOURCE_SETTINGS);
		startActivity(settingsIntent);
	}
}
