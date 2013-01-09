/**
 * 
 */
package com.ladicle.remoco.obj;

import android.content.SharedPreferences;

/**
 * @author Aya
 * 
 */
public class Network {
	public static final String PREF_KEY_TCPPORT = "NETWORK_TCPPORT_NUM";
	public static final String PREF_KEY_UDPPORT = "NETWORK_UDPPORT_NUM";
	private static final Network instance = new Network();
	private int udpPort;
	private int tcpPort;

	private Network() {}

	public void setData(SharedPreferences sp) {
		tcpPort = sp.getInt(PREF_KEY_TCPPORT, 60001);
		udpPort = sp.getInt(PREF_KEY_UDPPORT, 60002);
	}

	public static Network getInstance() {
		return instance;
	}

	public int getTcpPort() {
		return tcpPort;
	}

	public int getUdpPort() {
		return udpPort;
	}
}
