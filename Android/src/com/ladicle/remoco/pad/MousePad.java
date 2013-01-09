/**
 * 
 */
package com.ladicle.remoco.pad;

import android.os.Bundle;
import android.view.MotionEvent;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;
import android.widget.LinearLayout;

import com.ladicle.remoco.R;
import com.ladicle.remoco.activity.MainActivity;
import com.ladicle.remoco.net.IConnectionService;
import com.ladicle.remoco.obj.Display;

public class MousePad extends Pad {
	private static final String NAME = "Mouse Pad";
	IConnectionService net = MainActivity.mService;

	public MousePad() {
		super(NAME);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.pad_mouse);

		Display display = Display.getInstance();
		LinearLayout layout = (LinearLayout) findViewById(R.id.root_mouse);
		FrameLayout.LayoutParams lpParams = (LayoutParams) layout
				.getLayoutParams();
		lpParams.height = display.getHeight();
		layout.setLayoutParams(lpParams);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		log.i("onTouch");

		if (event.getPointerCount() == 1
				&& event.getAction() == MotionEvent.ACTION_DOWN) {
		}
		return false;
	}
}
