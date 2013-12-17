package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.SetGlobalPropertiesResponse;

public interface ISyncSetGlobalPropertiesResponseListener {
	public void onSetGlobalPropertiesResponse(SetGlobalPropertiesResponse response, Object tag);
}