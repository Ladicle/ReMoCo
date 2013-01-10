package com.ladicle.remoco.obj;

import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

public class Global {
	// Primary key
	private static final String PREF_KEY_TCPPORT = "NETWORK_TCPPORT_NUM";
	private static final String PREF_KEY_UDPPORT = "NETWORK_UDPPORT_NUM";
	private static final String PREF_KEY_WIDTH 	 = "DISPLAY_WIDTH";
	private static final String PREF_KEY_HEIGHT  = "DISPLAY_HEIGHT";

	// Display
	public static int WINDOW_WIDTH;
	public static int WINDOW_HEIGHT;

	// Network
	public static final int DEFAULT_TCP_PORT = 60002;
	public static final int DEFAULT_UDP_PORT = 60001;
	public static int TCP_PORT;
	public static int UDP_PORT;

	/** Save global values to Preference */
	public static void savePreference(Editor editor) {
		editor.putInt(PREF_KEY_WIDTH,	WINDOW_WIDTH);
		editor.putInt(PREF_KEY_HEIGHT, 	WINDOW_HEIGHT);
		editor.putInt(PREF_KEY_TCPPORT, TCP_PORT);
		editor.putInt(PREF_KEY_UDPPORT, UDP_PORT);
		editor.commit();
	}

	/**
	 * Load global values from Preference
	 * 
	 * if exist value -> return true; if not exist value -> return false;
	 * 
	 */
	public static boolean loadPreference(SharedPreferences sp) {
		if ((WINDOW_WIDTH = sp.getInt(PREF_KEY_WIDTH, -1)) == -1) {
			TCP_PORT = DEFAULT_TCP_PORT;
			UDP_PORT = DEFAULT_UDP_PORT;
			return false;
		}
		WINDOW_HEIGHT = sp.getInt(PREF_KEY_HEIGHT, 0);
		TCP_PORT = sp.getInt(PREF_KEY_TCPPORT, 60001);
		UDP_PORT = sp.getInt(PREF_KEY_UDPPORT, 60002);

		return true;
	}
}
