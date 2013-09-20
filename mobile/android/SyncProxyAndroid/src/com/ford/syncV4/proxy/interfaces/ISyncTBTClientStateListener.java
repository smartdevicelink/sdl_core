package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.OnTBTClientState;

public interface ISyncTBTClientStateListener {
	public void onOnTBTClientState(OnTBTClientState notification);
}