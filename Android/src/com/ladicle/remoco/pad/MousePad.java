package com.ladicle.remoco.pad;

import java.util.List;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.ladicle.remoco.R;
import com.ladicle.remoco.util.Global;

public class MousePad extends Pad {
	//Debug
	private static final String NAME = "Mouse Pad";
	
	//UI
	private ImageView leftButton, CenterButton, RightButton;
	
	//Sensor
	private SensorManager sensorManager;
	private Sensor accelerometer;
	private Sensor orientation;

	float x, y;
	int onmouse;
	String state;
	private final static String BR = System.getProperty("line.separator");
	private final static int WC = LinearLayout.LayoutParams.WRAP_CONTENT;
	private TextView textView; // テキストビュー
	private String[] message = new String[3];
	
	// センサーの値
	private float[] accelerometerValues = new float[3];// 加速度の値
	private boolean accelerometerEnabled = false; // 加速度センサーの有無
	private boolean mRegisteredSensor;
	private SensorManager mSensorManager;
	private float[] currentOrientationValues = { 0.0f, 0.0f, 0.0f };
	private float[] currentAccelerationValues = { 0.0f, 0.0f, 0.0f };

	// 回転行列
	private float[] inR = new float[16];
	private float[] outR = new float[16];
	private float[] I = new float[16];

	// センサーの値
	private float[] magneticValues = new float[3]; // 地磁気の値
	private float[] orientationValues = new float[3]; // 回転の値
	private boolean magneticEnabled = false; // 地磁気センサーの有無

	public MousePad() {
		super(NAME);
	}

	@Override
	protected void onPause() {
		super.onPause();
		sensorManager.unregisterListener((SensorListener) this);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.pad_mouse);

		LinearLayout layout = (LinearLayout) findViewById(R.id.root_mouse);
		FrameLayout.LayoutParams lpParams = (LayoutParams) layout
				.getLayoutParams();
		lpParams.height = Global.WINDOW_HEIGHT;
		layout.setLayoutParams(lpParams);

		mRegisteredSensor = false;
		mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
		findvews();
		settingViewEvent();

		// センサーマネージャの取得
		sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

		// センサーの取得
		List<Sensor> list;
		list = sensorManager.getSensorList(Sensor.TYPE_ACCELEROMETER);
		if (list.size() > 0)
			accelerometer = list.get(0);
		list = sensorManager.getSensorList(Sensor.TYPE_ORIENTATION);
		if (list.size() > 0)
			orientation = list.get(0);

		// センサーマネージャの取得
		sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
		onResume();
	}

	// アクティビティ再開時に呼ばれる
	@Override
	protected void onResume() {
		super.onResume();

		// センサーの取得
		List<Sensor> sensors = sensorManager.getSensorList(Sensor.TYPE_ALL);
		for (Sensor sensor : sensors) {
			int type = sensor.getType();
			// 加速度センサーのリスナー登録
			if (type == Sensor.TYPE_ACCELEROMETER) {
				sensorManager.registerListener((SensorEventListener) this, sensor,
						SensorManager.SENSOR_DELAY_UI);
				accelerometerEnabled = true;
			}
			// 地磁気センサーのリスナー登録
			if (type == Sensor.TYPE_MAGNETIC_FIELD) {
				sensorManager.registerListener((SensorEventListener) this, sensor,
						SensorManager.SENSOR_DELAY_UI);
				magneticEnabled = true;
			}
		}
	}

	@Override
	protected void onStop() {
		// アプリの停止
		super.onStop();

		// センサーのリスナー解除
		if (accelerometerEnabled && magneticEnabled) {
			sensorManager.unregisterListener((SensorListener) this);
			accelerometerEnabled = false;
			magneticEnabled = false;
		}
	}

	// 精度変更イベントの処理
	public void onSensorChanged(SensorEvent event) {

		if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
			currentOrientationValues[0] = event.values[0] * 0.1f
					+ currentOrientationValues[0] * (1.0f - 0.1f);
			currentOrientationValues[1] = event.values[1] * 0.1f
					+ currentOrientationValues[1] * (1.0f - 0.1f);
			currentOrientationValues[2] = event.values[2] * 0.1f
					+ currentOrientationValues[2] * (1.0f - 0.1f);

			currentAccelerationValues[0] = event.values[0]
					- currentOrientationValues[0];
			currentAccelerationValues[1] = event.values[1]
					- currentOrientationValues[1];
			currentAccelerationValues[2] = event.values[2]
					- currentOrientationValues[2];
			String orientationValue1 = String
					.valueOf(currentAccelerationValues[0]);
			String orientationValue2 = String
					.valueOf(currentAccelerationValues[1]);
			String orientationValue3 = String
					.valueOf(currentAccelerationValues[2]);
		}

		if (event.accuracy == SensorManager.SENSOR_STATUS_UNRELIABLE)
			return;

		// センサー種別の取得
		int type = event.sensor.getType();

		// 加速度の値の取得
		if (type == Sensor.TYPE_ACCELEROMETER) {
			accelerometerValues = event.values;
		}
		// 地磁気の値の取得
		else if (type == Sensor.TYPE_MAGNETIC_FIELD) {
			magneticValues = event.values;
		}
		// 回転の値の取得
		if (accelerometerEnabled && magneticEnabled) {
			SensorManager.getRotationMatrix(inR, I, accelerometerValues,
					magneticValues);
			SensorManager.remapCoordinateSystem(inR, SensorManager.AXIS_X,
					SensorManager.AXIS_Z, outR);
			SensorManager.getOrientation(outR, orientationValues);
		}
		String orientationValuesa = String.valueOf(orientationValues[1]);

		orientationValues[0] = (float) (9.80665 * Math
				.sin(orientationValues[0]));
		String orientationValuess = String.valueOf(orientationValues[0]);
		accelerometerValues[0] = (accelerometerValues[0] + orientationValues[0]);
		String accelerometerValue0 = String.valueOf(accelerometerValues[0]);
		String accelerometerValue1 = String.valueOf(accelerometerValues[1]);
		String orientationValue0 = String.valueOf(orientationValues[0]);
		String orientationValue1 = String.valueOf(orientationValues[1]);
		String orientationValue2 = String.valueOf(orientationValues[2]);

		for (int i = 0; i < 3; i++) {
			int w = (int) (10 * event.values[i]);
			event.values[i] = (float) (w / 10.0f);
			message[i] = String.valueOf(event.values[1]);
		}

		if (event.sensor == accelerometer) {
			
		}
		if (event.sensor == orientation) {

		}
	}

	public void onAccuracyChanged(Sensor sensor, int accuracy) {
	}

	private void settingViewEvent() {
		RightButton.setOnTouchListener(new OnTouchListener() {

			public boolean onTouch(View v, MotionEvent event) {
				onmouse = 2;
				if (event.getAction() == MotionEvent.ACTION_UP) {
					// タッチした時
					state = "right_up";
				} else if (event.getAction() == MotionEvent.ACTION_DOWN) {
					// 指を離した時
					x = -1;
					y = -1;
					state = "right_down";
				} else if (event.getAction() == MotionEvent.ACTION_MOVE) {
					// フリック(指を滑らす)した時
					x = event.getX();
					y = event.getY();
				}

				String sx = String.valueOf(x);
				String sy = String.valueOf(y);
				String ms = "1," + state;
				Log.d("ontouch", ms);

				Global.net.sendMessage(ms);
				
				return true;
			}
		});

		CenterButton.setOnTouchListener(new OnTouchListener() {

			public boolean onTouch(View v, MotionEvent event) {
				onmouse = 3;
				if (event.getAction() == MotionEvent.ACTION_DOWN) {
					// タッチした時
					state = "center_down";
				} else if (event.getAction() == MotionEvent.ACTION_UP) {
					// 指を離した時
					x = -2;
					y = -2;
					state = "center_up";
				} else if (event.getAction() == MotionEvent.ACTION_MOVE) {
					// フリック(指を滑らす)した時
					x = event.getX();
					y = event.getY();
					state = "center";
				}

				String sx = String.valueOf(x);
				String sy = String.valueOf(y);
				String ms = "1," + state + sy;
				Log.d("ontouch", ms);

				Global.net.sendMessage(ms);
				return true;
			}
		});

		leftButton.setOnTouchListener(new OnTouchListener() {

			public boolean onTouch(View v, MotionEvent event) {
				onmouse = 1;
				if (event.getAction() == MotionEvent.ACTION_DOWN) {
					// タッチした時
					state = "left_down";
				} else if (event.getAction() == MotionEvent.ACTION_UP) {
					// 指を離した時
					x = -1;
					y = -1;
					state = "left_up";
				}

				String sx = String.valueOf(x);
				String sy = String.valueOf(y);
				String ms = "1," + state;
				Log.d("ontouch", ms);
				
				Global.net.sendMessage(ms);
				return true;
			}
		});
	}

	private void findvews() {
		RightButton = (ImageView) findViewById(R.id.mouse_right);
		leftButton = (ImageView) findViewById(R.id.mouse_left);
		CenterButton = (ImageView) findViewById(R.id.mouse_center);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		check(Global.net.sendMessage("-2"));

		if (event.getPointerCount() == 1
				&& event.getAction() == MotionEvent.ACTION_DOWN) {
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
