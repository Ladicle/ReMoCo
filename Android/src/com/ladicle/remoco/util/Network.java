package com.ladicle.remoco.util;

import java.io.IOException;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;


public class Network {
	// Debug
	private static final String TAG = "Network";
	private static final MyLog log = new MyLog(TAG);
	
	// Instance
	private static final Network instance = new Network();

	// Network values
	private ServerSocket servers;
	private Socket socket;
	private OutputStream out;
	
	private Network() {}
	public static Network getInstance(){
		return instance;
	}

	/** Start up tcp server */
	public boolean initServer(){
		try {
			servers = new ServerSocket(Global.TCP_PORT);
			socket	= servers.accept();
			out		= socket.getOutputStream();
			
		} catch (IOException e) {
			log.e(e.getMessage());
			return false;
		}

		log.d("start tcp server.");
		return true;
	}
	
	
	/** Stop tcp server */
	public void stopServer(){
		try {
			sendMessage("-2");
			if(out!=null)	 out.close();
			if(socket!=null) socket.close();
			if(servers!=null)servers.close();
			
		} catch (IOException e) {
			log.e(e.getMessage());
		}
		
		log.d("close sockets");
	}
	
	
	/** Send message to PC */
	public boolean sendMessage(String message){
		if (socket != null && socket.isConnected()) {
			try {
				out.write(message.getBytes());
				out.flush();
				
			} catch (IOException e) {
				log.e(e.getMessage());
				return false;
			}
			
			log.i("send :" + message);
			return true;
		}
		return false;
	}
}
