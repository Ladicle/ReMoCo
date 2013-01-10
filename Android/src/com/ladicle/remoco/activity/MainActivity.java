package com.ladicle.remoco.activity;

import android.app.ActivityGroup;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.res.Resources;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.preference.PreferenceManager;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.TranslateAnimation;
import android.widget.Button;
import android.widget.LinearLayout;

import com.ladicle.remoco.R;
import com.ladicle.remoco.net.Connection;
import com.ladicle.remoco.net.IConnectionService;
import com.ladicle.remoco.net.IMassageCallback;
import com.ladicle.remoco.pad.MousePad;
import com.ladicle.remoco.pad.TenKeyPad;
import com.ladicle.util.MyLog;

public class MainActivity extends ActivityGroup {
	//Debug
	private static final String CLASS_NAME = "MainActivity";
	private static final MyLog log = new MyLog(CLASS_NAME);
	
	private static final String ACTIVITY_PAD = "PAD";
	private static final int RANGE = 5;
	
	private LinearLayout root, menu;
	private Resources resources;
	private Window window;
	private TranslateAnimation menuAnime;
	private int menuSize;
	private float menuX, menuMovement;
	private boolean showMenu = false;

	public static IConnectionService mService;
	private IMassageCallback mCallback = new IMassageCallback.Stub() {

		public void getMessage(String message) throws RemoteException {
			log.i("get messag :" + message);
		}
	};

	private ServiceConnection mServiceConnection = new ServiceConnection() {

		public void onServiceDisconnected(ComponentName name) {
			mService = null;
		}

		public void onServiceConnected(ComponentName name, IBinder service) {
			mService = IConnectionService.Stub.asInterface(service);
			try {
				mService.registerCallback(mCallback);
			} catch (RemoteException e) {
				log.e(e.getMessage().toString());
			}
		}
	};

	private enum PADS {
		MOUSE, TENKEY
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		resources = getResources();

		findViews();
		makeManuButton();

		// Add pad.
		addPad(PADS.MOUSE);

		// Bind
		bindService(new Intent(this, Connection.class), mServiceConnection,
				BIND_AUTO_CREATE);
	}

	private void addPad(PADS pad) {
		switch (pad) {
		case MOUSE:
			window = getLocalActivityManager().startActivity(ACTIVITY_PAD,
					new Intent(this, MousePad.class));
			break;
		case TENKEY:
			window = getLocalActivityManager().startActivity(ACTIVITY_PAD,
					new Intent(this, TenKeyPad.class));
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
		LinearLayout.LayoutParams param = new LinearLayout.LayoutParams(
				buttonSize, buttonSize);
		param.topMargin = buttonMargin;

		Button mouse = new Button(this);
		mouse.setLayoutParams(param);
		mouse.setBackgroundResource(R.drawable.ic_mouse);

		Button tenkey = new Button(this);
		tenkey.setLayoutParams(param);
		tenkey.setBackgroundResource(R.drawable.ic_tenkey);

		menu.addView(mouse);
		menu.addView(tenkey);

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
					} else if (0 >= move) {
						move = 0;
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
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		log.d("onDestroy");

		if (mService != null) {
			unbindService(mServiceConnection);
		}
	}
}
