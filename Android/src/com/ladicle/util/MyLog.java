/**
 * 
 */
package com.ladicle.util;

import android.util.Log;

/**
 * @author Aya
 * 
 */
public class MyLog {
	private String tag;
	private static boolean debug = true;

	public MyLog(String className) {
		tag = className;
	}

	public void d(String msg) {
		if (debug) {
			Log.d(tag, msg);
		}
	}

	public void d(String tag, String msg) {
		if (debug) {
			Log.d(this.tag, tag + ": " + msg);
		}
	}

	public void i(String msg) {
		if (debug) {
			Log.i(tag, msg);
		}
	}

	public void i(String tag, String msg) {
		if (debug) {
			Log.i(this.tag, tag + ": " + msg);
		}
	}

	public void e(String msg) {
		if (debug) {
			Log.e(tag, msg);
		}
	}

	public void e(String tag, String msg) {
		if (debug) {
			Log.e(this.tag, tag + ": " + msg);
		}
	}
}
