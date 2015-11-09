package com.example.camsrv;

import android.app.IntentService;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
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
		NotificationManager nm = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
		String action = intent.getAction();
		Log.d(TAG, action);
		if (action.equals("START")) {
			// Service starting. Create a notification.
			Notification notification = new Notification(R.drawable.ic_launcher, "CamSrv service running",
					System.currentTimeMillis());
			notification.setLatestEventInfo(this, "CamSrv Service", "Click to stop",
					PendingIntent.getService(this, 0, new Intent("STOP", null, this, this.getClass()), 0));
			notification.flags |= Notification.FLAG_ONGOING_EVENT;
			nm.notify(0, notification);

			result = AppNativeMain();
			Log.d(TAG, String.valueOf(result));
		} else if (action.equals("STOP")) {
			// User clicked the notification. Need to stop the service.
			nm.cancel(0);
			//stopSelf();

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
