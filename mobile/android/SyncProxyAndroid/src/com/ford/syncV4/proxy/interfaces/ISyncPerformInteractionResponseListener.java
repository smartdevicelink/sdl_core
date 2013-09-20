package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.PerformInteractionResponse;

public interface ISyncPerformInteractionResponseListener {
	public void onPerformInteractionResponse(PerformInteractionResponse response, Object tag);
}