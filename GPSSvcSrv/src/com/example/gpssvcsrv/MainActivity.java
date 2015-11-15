package com.example.gpssvcsrv;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.widget.SeekBar;

public class MainActivity extends Activity {

	private final static String TAG = "GPSSvcSrv MainActivity";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		Log.d(TAG, "onCreate()");

		Intent i = new Intent(MainActivity.this, GPSSvcSrv.class);
		i.setAction("START");
		startService(i);

		SeekBar seekBar1 = (SeekBar) findViewById(R.id.seekBar1);
		seekBar1.setProgress(50);
		seekBar1.setOnSeekBarChangeListener(new SlideToStartStop(this));

		//finish();
	}

	@Override
	public void onDestroy() {
		super.onDestroy(); // Always call the superclass method first.

		Log.d(TAG, "onDestroy()");
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}

	public class SlideToStartStop implements SeekBar.OnSeekBarChangeListener {

		int curProgress = 50;
		Activity mActivity;

		public SlideToStartStop(Activity activity) {
			mActivity = activity;			
		}

		@Override
		public void onStopTrackingTouch(SeekBar seekBar) {
			if (seekBar.getProgress() < 5) {
				Intent i = new Intent(MainActivity.this, GPSSvcSrv.class);
				i.setAction("STOP");
				startService(i);
			} else if (seekBar.getProgress() > 95) {
				Intent i = new Intent(MainActivity.this, GPSSvcSrv.class);
				i.setAction("START");
				startService(i);
			}			
			curProgress = 50;
			seekBar.setProgress(curProgress);			
		}

		@Override
		public void onStartTrackingTouch(SeekBar seekBar) {

		}

		@Override
		public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
			if (Math.abs(curProgress - progress) > 25) {
				seekBar.setProgress(curProgress);
			} else {
				curProgress = seekBar.getProgress();
			}
		}
	}
}
