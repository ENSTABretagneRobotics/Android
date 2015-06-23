package com.example.gpssms;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.location.GpsStatus;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.PowerManager;
import android.provider.Settings;
import android.telephony.SmsManager;
import android.view.Menu;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity {

	private PowerManager powerManager;
	private PowerManager.WakeLock sdwl;
	private PowerManager.WakeLock pwl;

	private LocationManager locationManager;
	private GpsStatus.NmeaListener nmeaListener;
	private LocationListener locationListener;

	double latitude;
	double longitude;
	double altitude;
	double speed;
	double bearing;

	String phone;
	String sms;
	int period;

	Timer updateTimer;
	
	TextView statusTextView;

	TextView latitudeTextView;
	TextView longitudeTextView;
	TextView altitudeTextView;
	TextView speedTextView;
	TextView bearingTextView;

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

		statusTextView = (TextView) findViewById(R.id.statusTextView);
		
		latitudeTextView = (TextView) findViewById(R.id.latitudeTextView);
		longitudeTextView = (TextView) findViewById(R.id.longitudeTextView);
		altitudeTextView = (TextView) findViewById(R.id.altitudeTextView);
		speedTextView = (TextView) findViewById(R.id.speedTextView);
		bearingTextView = (TextView) findViewById(R.id.bearingTextView);

		nmeaTextView = (TextView) findViewById(R.id.nmeaTextView);
		
		// Get the Location Manager.
		locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);

		// Define a listener that responds to NMEA updates. NMEA is the
		// low-level GPS protocol.
		nmeaListener = new GpsStatus.NmeaListener() {
			public void onNmeaReceived(long timestamp, String nmea) {
				nmeaTextView.setText(nmea);
			}
		};

		// Register the NMEA listener.
		locationManager.addNmeaListener(nmeaListener);

		// Define a listener that responds to location updates.
		locationListener = new LocationListener() {
			public void onLocationChanged(Location location) {
				// Called when a new location is found by the location provider.
				latitude = location.getLatitude();
				longitude = location.getLongitude();
				altitude = location.getAltitude();
				speed = location.getSpeed();
				bearing = location.getBearing();
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

		phone = null;
		sms = null;
		period = 0;
		
		statusTextView.setText("Status : Waiting for configuration");

		updateTimer = new Timer();
	}

	public void updateSMS(View view) {
		// Do something in response to button click.
		EditText phoneEditText = (EditText) findViewById(R.id.phoneEditText);
		EditText smsEditText = (EditText) findViewById(R.id.smsEditText);
		EditText periodEditText = (EditText) findViewById(R.id.periodEditText);
		try {
			phone = phoneEditText.getText().toString();
			sms = smsEditText.getText().toString();
			period = Integer.parseInt(periodEditText.getText().toString());
			if ((phone != null) && (phone.compareTo("") != 0) && (period >= 1)) {
				updateTimer.cancel();
				updateTimer.purge();
				updateTimer = new Timer();
				updateTimer.schedule(new updateTask(new Handler(), this), 0,
						period * 1000);
				statusTextView.setText("Status : Configured");
			} else {
				statusTextView.setText("Status : Bad configuration");
				updateTimer.cancel();
				updateTimer.purge();
			}
		} catch (Exception e) {
			statusTextView.setText("Status : Bad configuration");
			updateTimer.cancel();
			updateTimer.purge();
		}
	}

	public void update() {
		statusTextView.setText("Status : Sending");
		try {
			String text = sms + "\n" + latitude + "\n" + longitude + "\n"
					+ altitude + "\n" + speed + "\n" + bearing;
			SmsManager.getDefault().sendTextMessage(phone, null, text, null,
					null);
			statusTextView.setText("Status : Sent on "
					+ new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.US)
							.format(Calendar.getInstance().getTime()));
		} catch (Exception e) {
			statusTextView.setText("Status : Error");
		}
	}

	@Override
	public void onDestroy() {
		super.onDestroy(); // Always call the superclass method first.

		updateTimer.cancel();
		updateTimer.purge();

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

	private void enableLocationSettings() {
		Intent settingsIntent = new Intent(
				Settings.ACTION_LOCATION_SOURCE_SETTINGS);
		startActivity(settingsIntent);
	}
}
