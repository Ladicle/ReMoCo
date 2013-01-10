/**
 * 
 */
package com.ladicle.remoco.activity;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketException;

import android.app.Activity;
import android.content.Intent;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.View;
import android.widget.Toast;

import com.ladicle.remoco.R;
import com.ladicle.remoco.obj.Global;
import com.ladicle.util.MyLog;

public class SelectActivity extends Activity {
	// Debug
	private static final String CLASS_NAME = "SelectActivity";
	private static final MyLog log = new MyLog(CLASS_NAME);

	// Network
	private static final String UDP_IPADDRESS = "255.255.255.255";

	// GUI
	private static final Handler HANDLER = new Handler();

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_select);
		log.d("onCreate");
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.activity_select, menu);
		return true;
	}

	/** Called when clicked Wi-Fi button */
	public void onStartWifi(View view) {
		udpSend(int2Ipaddr(checkIPaddr()));
	}

	/** Called when clicked USB button */
	public void onStartUsb(View view) {
		goNextScene();
	}

	/** UDP broad cast and send device IPaddress */ 
	private void udpSend(final String sendMessage) {
		new Thread(new Runnable() {
			public void run() {
				InetSocketAddress addr = new InetSocketAddress(UDP_IPADDRESS,
						Global.UDP_PORT);
				DatagramPacket packet = null;

				try {
					packet = new DatagramPacket(sendMessage.getBytes(),
							sendMessage.length(), addr);
				} catch (SocketException e) {
					showErroMessage(e);
					return;
				}

				DatagramSocket sock = null;
				try {
					sock = new DatagramSocket();
				} catch (SocketException e) {
					showErroMessage(e);
					return;
				}

				try {
					sock.send(packet);
				} catch (IOException e) {
					showErroMessage(e);
					return;
				}

				// HANDLER.post(new Runnable() {
				// public void run() {
				// goNextScene();
				// }
				// });
				goNextScene();
			}
		}).start();
	}

	/** Show error message by Toast */
	private void showErroMessage(final Exception e) {
		HANDLER.post(new Runnable() {
			public void run() {
				Toast.makeText(SelectActivity.this, e.getMessage(),
						Toast.LENGTH_LONG).show();
			}
		});
	}

	/** Go to next scene */
	private void goNextScene() {
		log.d("goNextScene");
		startActivity(new Intent(this, MainActivity.class));
		finish();
	}

	/** Get device IPaddress */
	private int checkIPaddr() {
		WifiManager wm = (WifiManager) getSystemService(WIFI_SERVICE);
		WifiInfo wi = wm.getConnectionInfo();
		return wi.getIpAddress();
	}

	/** Change IPaddress type, Integer to String */
	private String int2Ipaddr(int addr) {
		return ((addr >> 0) & 0xFF) + "." + ((addr >> 8) & 0xFF) + "."
				+ ((addr >> 16) & 0xFF) + "." + ((addr >> 24) & 0xFF);
	}
}
