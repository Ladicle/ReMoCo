package com.ladicle.remoco.activity;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.widget.RelativeLayout;

import com.ladicle.remoco.R;
import com.ladicle.remoco.obj.Global;
import com.ladicle.util.MyLog;

public class TitleActivity extends Activity {
	// Debug
	private static final String CLASS_NAME = "TitleActivity";
	private static final MyLog log = new MyLog(CLASS_NAME);

	private RelativeLayout reLayout;
	private Editor editor;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_title);
		log.d("onCreate");

		//Load value from preference
		SharedPreferences sp = PreferenceManager
				.getDefaultSharedPreferences(this);
		if (Global.loadPreference(sp)) nextScene();

		reLayout = (RelativeLayout) findViewById(R.id.title_root);
		editor = sp.edit();
	}

	@Override
	public void onWindowFocusChanged(boolean hasFnocus) {
		super.onWindowFocusChanged(hasFnocus);

		//Save window size
		Global.WINDOW_WIDTH  = reLayout.getWidth();
		Global.WINDOW_HEIGHT = reLayout.getHeight();
		Global.savePreference(editor);
		
		nextScene();
	}

	/** Go to next scene */
	private void nextScene() {
		startActivity(new Intent(TitleActivity.this, SelectActivity.class));
		finish();
	}
}
