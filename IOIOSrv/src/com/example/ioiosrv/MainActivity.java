package com.example.ioiosrv;

import ioio.lib.api.AnalogInput;
import ioio.lib.api.DigitalInput;
import ioio.lib.api.DigitalOutput;
import ioio.lib.api.PulseInput;
import ioio.lib.api.PwmOutput;
import ioio.lib.api.exception.ConnectionLostException;
import ioio.lib.util.BaseIOIOLooper;
import ioio.lib.util.IOIOLooper;
import ioio.lib.util.android.IOIOActivity;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Arrays;
import java.util.TimerTask;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.content.Context;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.PowerManager;
import android.view.Menu;
import android.widget.ToggleButton;

public class MainActivity extends IOIOActivity {

	private PowerManager powerManager;
	private PowerManager.WakeLock sdwl;
	private PowerManager.WakeLock pwl;

	private ToggleButton button_;

	boolean do1 = false;
	boolean do2 = false;
	int pw1 = 1500;
	int pw2 = 1500;
	int pw3 = 1500;
	boolean di1 = false;
	boolean di2 = false;
	int pwIn1 = 1500;
	int pwIn2 = 1500;
	int pwIn3 = 1500;
	double voltage1 = 0;
	double voltage2 = 0;

	//Timer updateTimer;

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

		button_ = (ToggleButton) findViewById(R.id.button);

		//updateTimer = new Timer();
		//updateTimer.schedule(new updateTask(new Handler(), this), 0, 50);

		new NetworkTask().execute("");
	}

	@Override
	public void onDestroy() {
		super.onDestroy(); // Always call the superclass method first.

		// Cannot destroy NetworkTask properly, however it should fail
		// immediately if started twice...

		//updateTimer.cancel();
		
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
	}

	@Override
	protected void onStop() {
		super.onStop(); // Always call the superclass method first.
	}

	public void update() {
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
		private DigitalOutput do1_;
		private DigitalOutput do2_;
		private PwmOutput pwm1_;
		private PwmOutput pwm2_;
		private PwmOutput pwm3_;
		private DigitalInput di1_;
		private DigitalInput di2_;
		private PulseInput pwmIn1_;
		private PulseInput pwmIn2_;
		private PulseInput pwmIn3_;
		private AnalogInput ai1_;
		private AnalogInput ai2_;

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
			//do1_ = ioio_.openDigitalOutput(10);
			//do2_ = ioio_.openDigitalOutput(11);
			pwm1_ = ioio_.openPwmOutput(12, 50);
			pwm2_ = ioio_.openPwmOutput(13, 50);
			pwm3_ = ioio_.openPwmOutput(14, 50);
			//di1_ = ioio_.openDigitalInput(3);
			//di2_ = ioio_.openDigitalInput(4);
			//pwmIn1_ = ioio_.openPulseInput(5, PulseMode.POSITIVE);
			//pwmIn2_ = ioio_.openPulseInput(6, PulseMode.POSITIVE);
			//pwmIn3_ = ioio_.openPulseInput(7, PulseMode.POSITIVE);
			//ai1_ = ioio_.openAnalogInput(41);
			//ai2_ = ioio_.openAnalogInput(42);
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
			//do1_.write(do1);
			//do2_.write(do2);
			pwm1_.setPulseWidth(pw1);
			pwm2_.setPulseWidth(pw2);
			pwm3_.setPulseWidth(pw3);
			try {
				//di1 = di1_.read();
				//di2 = di2_.read();
				//pwIn1 = (int)(pwmIn1_.getDuration()*1000);
				//pwIn2 = (int)(pwmIn2_.getDuration()*1000);
				//pwIn3 = (int)(pwmIn3_.getDuration()*1000);
				//voltage1 = ai1_.getVoltage();
				//voltage2 = ai2_.getVoltage();
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

	class NetworkTask extends AsyncTask<String, Void, Integer> {

		protected Integer doInBackground(String... urls) {
			timeout = 5;
			srvport = 4004;
			return LaunchSingleCliTCPSrv(srvport, null);
		}

		protected void onPostExecute(Integer param) {
			// TODO: check this.exception
			// TODO: do something with the feed
		}
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
			System.out.println("inittcpsrv error : " + e);
			try {
				ssc.close();
			} catch (Exception e2) {
			}
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}

	public int releasetcpsrv() {

		try {
			ssc.close();
		} catch (Exception e) {
			System.out.println("releasetcpsrv error : " + e);
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}

	public int disconnectclifromtcpsrv() {

		try {
			// sc.shutdownOutput();
			// sc.shutdownInput();
			sc.close();
		} catch (Exception e) {
			System.out.println("disconnectclifromtcpsrv error : " + e);
			try {
				sc.close();
			} catch (Exception e2) {
			}
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
			} else {
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

			System.out.println("Accepted connection from " + s);
		} catch (Exception e) {
			System.out.println("waitforclifortcpsrv error : " + e);
			try {
				selector.close();
			} catch (Exception e2) {
			}
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}

	public int LaunchSingleCliTCPSrv(int port, Object param) {

		int iResult = EXIT_FAILURE;

		if (inittcpsrv("0.0.0.0", port, 1, DEFAULT_SOCK_TIMEOUT) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}

		for (;;) {
			iResult = waitforclifortcpsrv(DEFAULT_SOCK_TIMEOUT);
			switch (iResult) {
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
		// if (releasetcpsrv() != EXIT_SUCCESS)
		// {
		// return EXIT_FAILURE;
		// }

		// return EXIT_SUCCESS;
	}

	public int handlecli(Object param) {

		Selector selector = null;
		int num = 0;
		int offset = 0;
		byte[] b = new byte[256];
		boolean bFound = false;
		Pattern p = Pattern.compile("#(\\d+)P(\\d+)");

		Arrays.fill(b, (byte) 0);

		for (;;) {
			try {
				selector = Selector.open();

				// Set to non-blocking, so we can use select.
				sc.configureBlocking(false);
				sc.register(selector, SelectionKey.OP_READ);

				num = selector.select(timeout);

				selector.close();

				if (num != 0) {
					// Reset to blocking, so we can use blocking reads and
					// writes.
					sc.configureBlocking(true);
					Socket s = sc.socket();

					s.setSoTimeout(DEFAULT_SOCK_TIMEOUT);

					int nbBytesRead = s.getInputStream().read(b, offset, 256 - offset);

					if (nbBytesRead <= 0)
						throw new IOException("read() failed");

					offset += nbBytesRead;

					// Conversion from null terminated string to String.
					// int strlen = 0;
					// for (strlen = 0; strlen < b.length && b[strlen] != 0;
					// strlen++) { }
					// String str = new String(b, 0, strlen, "UTF-8");
					String str = new String(b, 0, offset, "UTF-8");
					//System.out.println(str);
					String s0 = new String(b, offset - nbBytesRead, nbBytesRead, "UTF-8");
					//System.err.print(s0);

					// Should make recvuntil()...
					
					if (!bFound) {
						// To improve...
						if (b[offset - nbBytesRead] != '#') {
							// Reset.
							bFound = false;
							offset = 0;
							Arrays.fill(b, (byte) 0);
						} else {
							bFound = true;
							System.out.println("Found #");
						}
					}
					if (bFound) {
						// To improve...
						if (b[offset - 1] == '\r') {
							try {
								boolean bValid = false;
								Matcher m = p.matcher(str);
								while (m.find()) {
									bValid = false;
									// System.out.print ("Found < " +
									// m.group());
									// System.out.println (" > from " +
									// m.start() + " to " + m.end());
									int channel = Integer.parseInt(m.group(1));
									int pw = Integer.parseInt(m.group(2));
									System.out.println("<" + channel + ">"
											+ "<" + pw + ">");
									switch (channel) {
									case 0:
										pw1 = pw;
										break;
									case 1:
										pw2 = pw;
										break;
									case 2:
										pw3 = pw;
										break;
									default:
										//throw new Exception("Unsupported channel");
										System.out.println("Unsupported channel. ");
										break;
									}
									bValid = true;
								}
								if (bValid) {
									System.out.println("Valid command. ");
								} else {
									throw new Exception("Incomplete command");
								}
							} catch (Exception e) {
								System.out.println("Invalid command : " + e
										+ ". ");
							}
							// Reset.
							bFound = false;
							offset = 0;
							Arrays.fill(b, (byte) 0);
						} else {
							if (offset >= 256) {
								System.out.println("Invalid command. ");
								// Reset.
								bFound = false;
								offset = 0;
								Arrays.fill(b, (byte) 0);
							}
						}
					}

					// Echo.
					// s.getOutputStream().write(b);
					// offset = 0;
					// Arrays.fill(b, (byte) 0);

					// try {
					// Thread.sleep(50);
					// } catch (InterruptedException e) {
					// }
				}
			} catch (Exception e) {
				System.out.println("handlecli error : " + e + ". ");
				try {
					selector.close();
				} catch (Exception e2) {
				}
				// Reset to default values.
				do1 = false;
				do2 = false;
				pw1 = 1500;
				pw2 = 1500;
				pw3 = 1500;
				di1 = false;
				di2 = false;
				pwIn1 = 1500;
				pwIn2 = 1500;
				pwIn3 = 1500;
				voltage1 = 0;
				voltage2 = 0;
				return EXIT_FAILURE;
			}
		}

		// Unreachable code.
		// return EXIT_SUCCESS;
	}
}
