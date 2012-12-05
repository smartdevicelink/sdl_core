package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.ResetGlobalPropertiesResponse;

public interface ISyncResetGlobalPropertiesListener {
	public void onResetGlobalPropertiesResponse(ResetGlobalPropertiesResponse response, Object tag);
}