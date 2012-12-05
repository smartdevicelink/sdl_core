package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.AlertResponse;

public interface ISyncAlertResponseListener {
	public void onAlertResponse(AlertResponse response, Object tag);
}