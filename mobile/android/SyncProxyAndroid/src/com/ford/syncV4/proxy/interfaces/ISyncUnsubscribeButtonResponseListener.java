package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.UnsubscribeButtonResponse;

public interface ISyncUnsubscribeButtonResponseListener {
	public void onUnsubscribeButtonResponse(UnsubscribeButtonResponse response, Object tag);
}