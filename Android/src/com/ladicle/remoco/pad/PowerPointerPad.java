package com.ladicle.remoco.pad;

import android.os.Bundle;
import android.os.SystemClock;
import android.view.View;
import android.widget.Chronometer;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;
import android.widget.LinearLayout;

import com.ladicle.remoco.R;
import com.ladicle.remoco.util.Global;
import com.ladicle.remoco.util.MyLog;

public class PowerPointerPad extends Pad {
	// Debug
	private static final String	CLASS_NAME	= PowerPointerPad.class.getSimpleName();
	private static final MyLog	log			= new MyLog(CLASS_NAME);

	// UI
	private Chronometer	chronometer;

	public PowerPointerPad() {
		super(CLASS_NAME);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.pad_powerpointer);
		log.d("onCreate");
		findViews();
	}

	private void findViews() {
		LinearLayout layout = (LinearLayout) findViewById(R.id.root_mouse);
		FrameLayout.LayoutParams lpParams = (LayoutParams) layout
				.getLayoutParams();
		lpParams.height = Global.WINDOW_HEIGHT;
		layout.setLayoutParams(lpParams);
		
		chronometer = (Chronometer) findViewById(R.id.timer);
	}

	public void onChangePage(View view) {
		if (Global.net == null) return;

		String sendMessage = (String) view.getTag();
		if (!sendMessage.equals("")) {
			check(Global.net.sendMessage(sendMessage));
		}
	}

	public void onStart(View view) {
		if (Global.net == null) return;

		String sendMessage = (String) view.getTag();
		if (!sendMessage.equals("")) {
			check(Global.net.sendMessage(sendMessage));
			chronometer.setBase(SystemClock.elapsedRealtime());
			chronometer.start();			
		}
	}

	public void onStop(View view) {
		if (Global.net == null) return;

		String sendMessage = (String) view.getTag();
		if (!sendMessage.equals("")) {
			check(Global.net.sendMessage(sendMessage));
			chronometer.stop();
		}
	}

	private void check(boolean process) {
		if (!process && Global.net != null) {
			Global.net.stopServer();
			Global.net = null;
			finish();
		}
	}
}
