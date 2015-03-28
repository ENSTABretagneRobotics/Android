package com.example.camsrv;

import android.app.IntentService;
import android.content.Intent;
import android.util.Log;

public class AppNativeService extends IntentService {

	private final static String TAG = "AppNativeService";
	private int result;

	public AppNativeService() {
		super(TAG);
	}

	@Override
	protected void onHandleIntent(Intent intent) {

		String action = intent.getAction();
		Log.d(TAG, action);
		if (action.equals("START")) {
			result = AppNativeMain();
			Log.d(TAG, String.valueOf(result));
		} else if (action.equals("STOP")) {
			result = AppNativeQuit();
			Log.d(TAG, String.valueOf(result));
			stopSelf();
		}
	}

	public native int AppNativeMain();

	public native int AppNativeQuit();

	static {
		System.loadLibrary("appnativecpp_opencv_jni");
	}
}
