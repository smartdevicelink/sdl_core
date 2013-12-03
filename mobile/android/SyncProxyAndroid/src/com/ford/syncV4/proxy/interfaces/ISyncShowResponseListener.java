package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.ShowResponse;

public interface ISyncShowResponseListener {
	public void onShowResponse(ShowResponse response, Object tag);
}