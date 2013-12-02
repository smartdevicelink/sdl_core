package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.EncodedSyncPDataResponse;

public interface ISyncEncodedSyncPDataResponseListener {
	public void onEncodedSyncPDataResponse(EncodedSyncPDataResponse response, Object tag);
}