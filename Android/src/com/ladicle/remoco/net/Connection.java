/**
 * 
 */
package com.ladicle.remoco.net;

import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

import android.app.IntentService;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteCallbackList;
import android.os.RemoteException;

import com.ladicle.remoco.obj.Network;
import com.ladicle.util.MyLog;

/**
 * @author Aya
 * 
 */
public class Connection extends IntentService {
	private static final String TAG = "Connection";
	private static final MyLog log = new MyLog(TAG);
	private Network net = Network.getInstance();

	// Network values
	private ServerSocket servers;
	private Socket socket;
	// private InputStream in;
	private OutputStream out;

	public Connection() {
		super(TAG);
	}

	@Override
	protected void onHandleIntent(Intent intent) {
		try {
			servers = new ServerSocket(net.getTcpPort());
			log.i("Set up server.");
			socket = servers.accept();
			log.i("Connected to " + socket.getInetAddress());
			// in = socket.getInputStream();
			out = socket.getOutputStream();

		} catch (Exception e) {
			log.e(e.getMessage());
		}
	}

	@Override
	public IBinder onBind(Intent intent) {
		log.i("onBind");
		new Thread(new Runnable() {

			public void run() {
				try {
					servers = new ServerSocket(60002);
					log.i("Set up server.");
					socket = servers.accept();
					log.i("Connected to " + socket.getInetAddress());
					// in = socket.getInputStream();
					out = socket.getOutputStream();

				} catch (Exception e) {
					log.e(e.getMessage());
				}
			}
		}).start();

		return mStub;
	};

	private RemoteCallbackList<IMassageCallback> mCallbackList = new RemoteCallbackList<IMassageCallback>();
	private IConnectionService.Stub mStub = new IConnectionService.Stub() {

		public void unregisterCallback(IMassageCallback callback)
				throws RemoteException {
			log.i("unregister callback");
			mCallbackList.unregister(callback);
		}

		public void registerCallback(IMassageCallback callback)
				throws RemoteException {
			log.i("register callback");
			mCallbackList.register(callback);
		}

		public void sendMessage(String message) throws RemoteException {
			try {
				if (socket != null && socket.isConnected()) {
					out.write(message.getBytes());
					out.flush();
					log.i("send :" + message);
				} else {
					log.e("soket is null.");
				}
			} catch (Exception e) {
				log.e(e.getMessage());
			}
		}

	};
}
