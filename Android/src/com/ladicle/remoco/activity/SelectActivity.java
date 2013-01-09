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
import android.view.Menu;
import android.view.View;

import com.ladicle.remoco.R;
import com.ladicle.remoco.obj.Network;
import com.ladicle.util.MyLog;

public class SelectActivity extends Activity {
	private static final String CLASS_NAME = SelectActivity.class
			.getSimpleName();
	private static final MyLog log = new MyLog(CLASS_NAME);

	private Network net = Network.getInstance();

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_select);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.activity_select, menu);
		return true;
	}

	public void onStartWifi(View view) {
		udpSend(int2Ipaddr(checkIPaddr()));
		goNextScene();
	}

	public void onStartUsb(View view) {
		// TODO: click usb button.
		goNextScene();
	}

	private static final String IP_ADDR = "255.255.255.255";
	private boolean flag = true;

	private void udpSend(final String sendMessage) {
		new Thread(new Runnable() {
			public void run() {
				InetSocketAddress addr = new InetSocketAddress(IP_ADDR,
						60001);
				DatagramPacket packet = null;
				try {
					packet = new DatagramPacket(sendMessage.getBytes(),
							sendMessage.length(), addr);
				} catch (SocketException e1) {
					log.e("DatagramError:" + e1.getMessage());
				}

				if(packet==null) {
					log.e("packet is null");
				}

				DatagramSocket sock=null;
				try {
					sock = new DatagramSocket();
				} catch (SocketException e1) {
					log.e("DatagramSocket:" + e1.getMessage());
				}
				if(sock==null) {
					log.e("socket is null");
				}

				try {
					sock.send(packet);
				} catch (IOException e) {
					log.e("send:"+e.getMessage());
				}
			}
		}).start();
	}

	private void goNextScene() {
		log.d("goNextScene");
		startActivity(new Intent(this, MainActivity.class));
		finish();
	}

	private int checkIPaddr() {
		WifiManager wm = (WifiManager) getSystemService(WIFI_SERVICE);
		WifiInfo wi = wm.getConnectionInfo();
		return wi.getIpAddress();
	}

	private String int2Ipaddr(int addr) {
		return ((addr >> 0) & 0xFF) + "." + ((addr >> 8) & 0xFF) + "."
				+ ((addr >> 16) & 0xFF) + "." + ((addr >> 24) & 0xFF);
	}
}
