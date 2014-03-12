package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.SyncPDataResponse;

public interface ISyncSyncPDataResponseListener {
	public void onSyncPDataResponse(SyncPDataResponse response, Object tag);
}