package com.example.camsvcsrv;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.IBinder;
import android.os.PowerManager;
import android.util.Log;

public class CamSvcSrv extends Service {
	private final static String TAG = "CamSvcSrv";
	
	protected boolean bStop = true;
	protected boolean bNetworkTaskRunning = false;

	private NotificationManager nm;

	private PowerManager powerManager;
	private PowerManager.WakeLock sdwl;
	private PowerManager.WakeLock pwl;
	
	private int result;

//	public CamSvcSrv() {
//		super(TAG);
//		
//		Log.d(TAG, "CamSvcSrv()");		
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
			Log.d(TAG, "Starting CamSvcSrv service");			
			Notification notification = new Notification(R.drawable.ic_launcher, "CamSvcSrv service running",
					System.currentTimeMillis());
			notification.setLatestEventInfo(this, "CamSvcSrv Service", "Click to stop",
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
			
			bStop = false;
			
			
			//result = AppNativeMain();
			//Log.d(TAG, String.valueOf(result));
			
			
			new NetworkTask().execute("");
			
			Log.d(TAG, "CamSvcSrv service started");
		} else if ((!bStop)&&(action.equals("STOP"))) {
			// User clicked the notification. Need to stop the service.
			Log.d(TAG, "Stopping CamSvcSrv service");

			// Should stop the NetworkTask.
			
			
			result = AppNativeQuit();
			Log.d(TAG, String.valueOf(result));
			
			
			bStop = true;
			while (bNetworkTaskRunning)	{
				try {
					Thread.sleep(50);
				} catch (InterruptedException e) {
				}		
			}
			
			pwl.release();
			sdwl.release();
			
			nm.cancel(1);
			stopForeground(true);

			Log.d(TAG, "CamSvcSrv service stopped");
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
			return AppNativeMain();
		}

		protected void onPostExecute(Integer param) {	
			bNetworkTaskRunning = false;
		}
	}

	public native int AppNativeMain();

	public native int AppNativeQuit();

	static {
		System.loadLibrary("appnativecpp_opencv_jni");
	}
}
