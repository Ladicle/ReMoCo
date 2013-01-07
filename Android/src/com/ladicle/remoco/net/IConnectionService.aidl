package com.ladicle.remoco.net;

import com.ladicle.remoco.net.IMassageCallback;

interface IConnectionService {

	oneway void registerCallback(IMassageCallback callback);
	oneway void unregisterCallback(IMassageCallback callback);

	void sendMessage(String message);
}
