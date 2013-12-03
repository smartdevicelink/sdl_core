package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.SpeakResponse;

public interface ISyncSpeakResponseListener {
	public void onSpeakResponse(SpeakResponse response, Object tag);
}