package com.ladicle.remoco.pad;

import android.app.Activity;
import android.os.Bundle;

import com.ladicle.remoco.util.MyLog;

public class Pad extends Activity {
	protected MyLog log;

	public Pad(String name) {
		log = new MyLog(name);
	}

	@Override
	protected void onStart() {
		super.onStart();
		log.d("onStart");
	}

	@Override
	protected void onResume() {
		super.onResume();
		log.d("onResume");
	}

	@Override
	protected void onPause() {
		super.onPause();
		log.d("onPause");
	}

	@Override
	protected void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);
		log.d("onSaveInstanceState");
	}

	@Override
	protected void onRestoreInstanceState(Bundle savedInstanceState) {
		super.onRestoreInstanceState(savedInstanceState);
		log.d("onRestoreInstanceState");
	}

	@Override
	protected void onStop() {
		super.onStop();
		log.d("onStop");
	}

	@Override
	protected void onRestart() {
		super.onRestart();
		log.d("onRestart");
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		log.d("onDestroy");

		//finish();
	}
}
