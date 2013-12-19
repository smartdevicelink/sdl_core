package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.SetMediaClockTimerResponse;

public interface ISyncSetMediaClockTimerResponseListener {
	public void onSetMediaClockTimerResponse(SetMediaClockTimerResponse response, Object tag);
}