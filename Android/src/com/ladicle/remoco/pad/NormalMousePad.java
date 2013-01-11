package com.ladicle.remoco.pad;

import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;
import android.widget.ImageView;
import android.widget.LinearLayout;

import com.ladicle.remoco.R;
import com.ladicle.remoco.util.Global;

public class NormalMousePad extends Pad implements OnTouchListener {
	// Debug
	private static final String NAME = "Mouse Pad";

	// UI
	private ImageView leftButton, CenterButton, RightButton;
	private LinearLayout moveField;

	// Mouse
	private View  view;
	private float center_y;

	public NormalMousePad() {
		super(NAME);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.pad_mouse);
		findvews();
	}

	private void findvews() {
		LinearLayout layout = (LinearLayout) findViewById(R.id.root_mouse);
		FrameLayout.LayoutParams lpParams = (LayoutParams) layout
				.getLayoutParams();
		lpParams.height = Global.WINDOW_HEIGHT;
		layout.setLayoutParams(lpParams);

		leftButton	= (ImageView) findViewById(R.id.mouse_left);
		RightButton = (ImageView) findViewById(R.id.mouse_right);
		CenterButton= (ImageView) findViewById(R.id.mouse_center);
		moveField	= (LinearLayout) findViewById(R.id.mouse_field);

		moveField.setOnTouchListener(this);
		leftButton.setOnTouchListener(this);
		RightButton.setOnTouchListener(this);
		CenterButton.setOnTouchListener(this);
	}

	public boolean onTouch(View v, MotionEvent event) {
		this.view = v;
		return false;
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if (event.getPointerCount() !=1 && Global.net != null)
			return false;

		switch (event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			if(view==RightButton)	check(Global.net.sendMessage("1,rd"));
			if(view==CenterButton)	center_y=event.getY();
			if(view==leftButton)	check(Global.net.sendMessage("1,ld!"+event.getX()+"!"+event.getY()));
			if(view==moveField)		check(Global.net.sendMessage("5,"+event.getX()+","+event.getY()));
			return true;

		case MotionEvent.ACTION_UP:
			if(view==RightButton)	check(Global.net.sendMessage("1,ru"));
			if(view==leftButton)	check(Global.net.sendMessage("1,lu"));
			return true;

		case MotionEvent.ACTION_MOVE:
			if(view==moveField || view==leftButton) {
				check(Global.net.sendMessage("2,"+event.getX()+","+event.getY()));
			}
			if(view==CenterButton){
				float diff = (center_y-event.getY())*8;
				check(Global.net.sendMessage("1,cm!"+diff));
				center_y=event.getY();					
			}
			return true;

		default:
			return false;
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
