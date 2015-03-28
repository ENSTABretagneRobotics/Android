package ioio.examples.hello;

import ioio.lib.api.DigitalOutput;
import ioio.lib.api.PwmOutput;
import ioio.lib.api.exception.ConnectionLostException;
import ioio.lib.util.BaseIOIOLooper;
import ioio.lib.util.IOIOLooper;
import ioio.lib.util.android.IOIOActivity;
import android.os.Bundle;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
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
	private ToggleButton button_;
	private SeekBar seekBar1_;
	private SeekBar seekBar2_;
	private SeekBar seekBar3_;
	public int pw1 = 1500;
	public int pw2 = 1500;
	public int pw3 = 1500;

	/**
	 * Called when the activity is first created. Here we normally initialize
	 * our GUI.
	 */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		button_ = (ToggleButton) findViewById(R.id.button);
		seekBar1_ = (SeekBar) findViewById(R.id.seekBar1);
		seekBar1_.setProgress(50);
		seekBar2_ = (SeekBar) findViewById(R.id.seekBar2);
		seekBar2_.setProgress(50);
		seekBar3_ = (SeekBar) findViewById(R.id.seekBar3);
		seekBar3_.setProgress(50);

		OnSeekBarChangeListener onSeekBarChangeListener1 = new OnSeekBarChangeListener() {
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				pw1 = 1000 + progress * 10;
			}

			public void onStartTrackingTouch(SeekBar seekBar) {
			}

			public void onStopTrackingTouch(SeekBar seekBar) {
			}
		};
		OnSeekBarChangeListener onSeekBarChangeListener2 = new OnSeekBarChangeListener() {
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				pw2 = 1000 + progress * 10;
			}

			public void onStartTrackingTouch(SeekBar seekBar) {
			}

			public void onStopTrackingTouch(SeekBar seekBar) {
			}
		};
		OnSeekBarChangeListener onSeekBarChangeListener3 = new OnSeekBarChangeListener() {
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				pw3 = 1000 + progress * 10;
			}

			public void onStartTrackingTouch(SeekBar seekBar) {
			}

			public void onStopTrackingTouch(SeekBar seekBar) {
			}
		};

		seekBar1_.setOnSeekBarChangeListener(onSeekBarChangeListener1);
		seekBar2_.setOnSeekBarChangeListener(onSeekBarChangeListener2);
		seekBar3_.setOnSeekBarChangeListener(onSeekBarChangeListener3);
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
				Thread.sleep(100);
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
}