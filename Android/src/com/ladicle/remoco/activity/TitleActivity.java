package com.ladicle.remoco.activity;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.widget.RelativeLayout;

import com.ladicle.remoco.R;
import com.ladicle.remoco.obj.Display;
import com.ladicle.util.MyLog;

public class TitleActivity extends Activity {
	private static final String CLASS_NAME = "TitleActivity";
	private static final MyLog log = new MyLog(CLASS_NAME);
	private static final Display display = Display.getInstance();
	private RelativeLayout reLayout;
	private Editor editor;
	private boolean flag;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_title);
		log.d("onCreate");

		SharedPreferences sp = PreferenceManager
				.getDefaultSharedPreferences(this);

		if (display.getWidth() != 0) {
			display.setHeight(sp.getInt(Display.PREF_KEY_HEIGHT, 0));

			startActivity(new Intent(TitleActivity.this, SelectActivity.class));
			finish();
		}
		reLayout = (RelativeLayout) findViewById(R.id.title_root);
		editor = sp.edit();
	}

	@Override
	public void onWindowFocusChanged(boolean hasFnocus) {
		super.onWindowFocusChanged(hasFnocus);

		if (flag) {
			Display.savePreference(editor, reLayout.getWidth(),
					reLayout.getHeight());
		}
	}
}
