package com.ladicle.remoco.pad;

import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;
import android.widget.LinearLayout;

import com.ladicle.remoco.R;
import com.ladicle.remoco.util.Global;

public class TouchPad extends Pad implements OnTouchListener {
	// Debug
	private static final String	NAME	= "Mouse Pad";

	// Mouse
	private float				center_y;

	public TouchPad() {
		super(NAME);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.pad_blank);
		findvews();
	}

	private void findvews() {
		LinearLayout layout = (LinearLayout) findViewById(R.id.root_mouse);
		FrameLayout.LayoutParams lpParams = (LayoutParams) layout
				.getLayoutParams();
		lpParams.height = Global.WINDOW_HEIGHT;
		layout.setLayoutParams(lpParams);
		layout.setOnTouchListener(this);
	}

	private long clickTime;
	public boolean onTouch(View v, MotionEvent event) {
		if (Global.net == null) return false;
		if (event.getPointerCount() == 1) {
			switch (event.getAction()) {
				case MotionEvent.ACTION_DOWN:
					check(Global.net.sendMessage("5,"+event.getX()+","+event.getY()));
					clickTime = System.currentTimeMillis();
					center_y = event.getY();
					return true;

				case MotionEvent.ACTION_UP:
					if((System.currentTimeMillis() - clickTime)<100) {
						check(Global.net.sendMessage("1,lc"));
					}
					return true;

				case MotionEvent.ACTION_MOVE:
					check(Global.net.sendMessage("2,"+event.getX()+","+event.getY()));
					return true;

				default:
					return false;
			}
		}
		if (event.getPointerCount() == 2) {
			if(event.getAction()==MotionEvent.ACTION_MOVE) {
					float diff = (center_y - event.getY()) * 8;
					check(Global.net.sendMessage("1,cm!" + diff));
					center_y = event.getY();
					return true;
			}
		}
		return false;
	}

	private void check(boolean process) {
		if (!process && Global.net != null) {
			Global.net.stopServer();
			Global.net = null;
			finish();
		}
	}
}
