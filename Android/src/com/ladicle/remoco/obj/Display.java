package com.ladicle.remoco.obj;

import android.content.SharedPreferences.Editor;

public class Display {
	// PREFERENCE KEY
	public static final String PREF_KEY_WIDTH = "DISPLAY_WIDTH";
	public static final String PREF_KEY_HEIGHT = "DISPLAY_HEIGHT";

	private static final Display instance = new Display();

	private int width;
	private int height;

	private Display() {
	};

	public static Display getInstance() {
		return instance;
	}

	public int getWidth() {
		return width;
	}

	public void setWidth(int width) {
		this.width = width;
	}

	public int getHeight() {
		return height;
	}

	public void setHeight(int height) {
		this.height = height;
	}

	public static void savePreference(Editor editor, int width, int height) {
		editor.putInt(PREF_KEY_WIDTH, width);
		editor.putInt(PREF_KEY_HEIGHT, height);
		editor.commit();
	}
}
