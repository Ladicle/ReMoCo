package com.ladicle.remoco.pad;

import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;
import android.widget.TableLayout;
import android.widget.TextView;

import com.ladicle.remoco.R;
import com.ladicle.remoco.util.Global;
import com.ladicle.remoco.util.MyLog;

public class TenKeyPad extends Pad implements OnTouchListener {

	private static final String CLASS_NAME = TenKeyPad.class.getSimpleName();
	private static final MyLog log = new MyLog(CLASS_NAME);
	private int[] buttonID = { 
			R.id.tenky_btn0, R.id.tenky_btn1,  R.id.tenky_btn2, 
			R.id.tenky_btn3, R.id.tenky_btn4,  R.id.tenky_btn5,
			R.id.tenky_btn6, R.id.tenky_btn7,  R.id.tenky_btn8, 
			R.id.tenky_btn9, R.id.tenky_btn10, R.id.tenky_btn11,
			R.id.tenky_btn12,R.id.tenky_btn13, R.id.tenky_btn14,
			R.id.tenky_btn15,R.id.tenky_btn16, R.id.tenky_btn17,
			R.id.tenky_btn18,R.id.tenky_btn19, R.id.tenky_btn20,
			R.id.tenky_btn21,R.id.tenky_btn22, R.id.tenky_btn23 };
	
	private TextView[] buttons = new TextView[buttonID.length];
	private View v;
	
	public TenKeyPad() {
		super("Tenkey");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.pad_tenkey);
		log.d("onCreate");
		setViewSize();
		findViews();
	}

	/**
	 * Setting View size.
	 */
	private void setViewSize() {
		TableLayout layout = (TableLayout) findViewById(R.id.root_tenkey);
		FrameLayout.LayoutParams lpParams = (LayoutParams) layout
				.getLayoutParams();
		lpParams.height = Global.WINDOW_HEIGHT;
		layout.setLayoutParams(lpParams);
	}

	/**
	 * Set tenkey button.
	 */
	private void findViews() {
		int length = buttonID.length;
		for (int i = 0; i < length; ++i) {
			buttons[i] = (TextView) findViewById(buttonID[i]);
			buttons[i].setOnTouchListener(this);
		}
	}

	public boolean onTouch(View v, MotionEvent event) {
		this.v = v;
		return false;
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if (event.getPointerCount() !=1 && Global.net != null)
			return false;
		
		if(event.getAction()==MotionEvent.ACTION_DOWN) {
			String sendMessage = (String) v.getTag();
			if(!sendMessage.equals("")) {
				check(Global.net.sendMessage(sendMessage));
			}
			if(v!=buttons[19] && v!=buttons[23]) {
				v.setBackgroundResource(R.drawable.background_gradlight_up);
			}
			return true;
		}
		
		if(event.getAction()==MotionEvent.ACTION_UP) {
			if(v!=buttons[19] && v!=buttons[23]) {
				v.setBackgroundResource(R.drawable.background_gradlight_up2);
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
