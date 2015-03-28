package com.example.camsrv;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.PowerManager;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {

	private PowerManager powerManager;
	private PowerManager.WakeLock sdwl;
	private PowerManager.WakeLock pwl;

	private Button startButton;
	private Button stopButton;
	private TextView infoTextView;

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

		startButton = (Button) findViewById(R.id.startButton);
		stopButton = (Button) findViewById(R.id.stopButton);
		infoTextView = (TextView) findViewById(R.id.infoTextView);

		final Intent i = new Intent(MainActivity.this, AppNativeService.class);

		infoTextView.setText("STARTING...");
		i.setAction("START");
		startService(i);
		infoTextView.setText("STARTED");

		// AppNativeQuit() does not seem to be able to stop the native
		// code...

		startButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				infoTextView.setText("STARTING...");
				i.setAction("START");
				startService(i);
				infoTextView.setText("STARTED");
			}
		});
		stopButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				infoTextView.setText("STOPPING...");
				i.setAction("STOP");
				stopService(i);
				try {
					Thread.sleep(15000);
				} catch (InterruptedException e) {
				}
				infoTextView.setText("STOPPED");
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}
}
