package com.ladicle.remoco.activity;

import android.app.ActivityGroup;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.TranslateAnimation;
import android.widget.Button;
import android.widget.LinearLayout;

import com.ladicle.remoco.R;
import com.ladicle.remoco.pad.NormalMousePad;
import com.ladicle.remoco.pad.PowerPointerPad;
import com.ladicle.remoco.pad.TenKeyPad;
import com.ladicle.remoco.pad.TouchPad;
import com.ladicle.remoco.util.Global;
import com.ladicle.remoco.util.MyLog;
import com.ladicle.remoco.util.Network;

public class ControllerActivity extends ActivityGroup {
	// Debug
	private static final String	CLASS_NAME		= "MainActivity";
	private static final MyLog	log				= new MyLog(CLASS_NAME);

	private static final String	ACTIVITY_PAD	= "PAD";
	private static final int	RANGE			= 5;

	// UI
	private Button				mouse, tenkey, touch, power;
	private LinearLayout		root, menu;
	private Resources			resources;
	private Window				window;
	private TranslateAnimation	menuAnime;
	private int					menuSize;
	private float				menuX, menuMovement;
	private boolean				showMenu		= false;

	private enum PADS {
		MOUSE, TENKEY, TOUCH, POWER
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		Global.net = Network.getInstance();

		new Thread(new Runnable() {
			public void run() {check(Global.net.initServer());}
		}).start();

		resources = getResources();
		findViews();
		makeManuButton();

		// Add pad.
		addPad(PADS.MOUSE);
		log.d("onCreate");
	}

	private void addPad(PADS pad) {
		switch (pad) {
			case MOUSE:
				window = getLocalActivityManager().startActivity(ACTIVITY_PAD, new Intent(this, NormalMousePad.class));
				break;

			case TENKEY:
				window = getLocalActivityManager().startActivity(ACTIVITY_PAD, new Intent(this, TenKeyPad.class));
				break;

			case TOUCH:
				window = getLocalActivityManager().startActivity(ACTIVITY_PAD, new Intent(this, TouchPad.class));
				break;
				
			case POWER:
				window = getLocalActivityManager().startActivity(ACTIVITY_PAD, new Intent(this, PowerPointerPad.class));
				break;

			default:
				break;
		}
		root.removeAllViews();
		root.addView(window.getDecorView());
	}

	private void findViews() {
		root = (LinearLayout) findViewById(R.id.main_pad);
		menu = (LinearLayout) findViewById(R.id.main_menu);
	}

	private void makeManuButton() {
		int buttonSize = (int) resources.getDimension(R.dimen.menu_ic_size);
		int buttonMargin = (int) resources.getDimension(R.dimen.menu_ic_margin);

		// Layout setting
		LinearLayout.LayoutParams param = new LinearLayout.LayoutParams(buttonSize, buttonSize);
		param.topMargin = buttonMargin;

		mouse = new Button(this);
		mouse.setLayoutParams(param);
		mouse.setBackgroundResource(R.drawable.ic_mouse);

		tenkey = new Button(this);
		tenkey.setLayoutParams(param);
		tenkey.setBackgroundResource(R.drawable.ic_tenkey);

		touch = new Button(this);
		touch.setLayoutParams(param);
		touch.setBackgroundResource(R.drawable.ic_touch);
		
		power = new Button(this);
		power.setLayoutParams(param);
		power.setBackgroundResource(R.drawable.ic_powerpointer);

		menu.addView(mouse);
		menu.addView(tenkey);
		menu.addView(touch);
		menu.addView(power);

		mouse.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				addPad(PADS.MOUSE);
			}
		});
		tenkey.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				addPad(PADS.TENKEY);
			}
		});
		touch.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				addPad(PADS.TOUCH);
			}
		});
		power.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				addPad(PADS.POWER);
			}
		});		

		buttonClickable(false);
	}

	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);
		if (!showMenu) {
			showMenu = true;
			menuSize = menu.getWidth();

			menuAnime = new TranslateAnimation(0, menuSize, 0, 0);
			menuAnime.setFillAfter(true);
			menuAnime.setDuration(500);
			menuAnime.setInterpolator(new AccelerateInterpolator());
			menu.startAnimation(menuAnime);
		}
	}

	private void buttonClickable(boolean clickable) {
		mouse.setClickable(clickable);
		tenkey.setClickable(clickable);
		touch.setClickable(clickable);
		power.setClickable(clickable);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int count = event.getPointerCount();
		int action = event.getAction();

		if (count == 3) {
			if (action == MotionEvent.ACTION_DOWN) {
				menuX = event.getX();
				menuMovement = menuSize;

			} else if (action == MotionEvent.ACTION_MOVE) {
				float offset = event.getX() - menuX;
				menuX = event.getX();

				if (offset > RANGE || offset < RANGE) {
					float move = menuMovement + offset;
					if (menuSize <= move) {
						move = menuSize;
						buttonClickable(false);

					} else if (0 >= move) {
						move = 0;
						buttonClickable(true);
					}

					menuAnime = new TranslateAnimation(menuMovement, move, 0, 0);
					menuAnime.setFillAfter(true);
					menu.startAnimation(menuAnime);

					menuMovement = move;
				}
			}
		}

		return super.onTouchEvent(event);
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK | keyCode == KeyEvent.KEYCODE_HOME) {
			Global.net.stopServer();
			Global.net = null;
			finish();
			return true;
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
