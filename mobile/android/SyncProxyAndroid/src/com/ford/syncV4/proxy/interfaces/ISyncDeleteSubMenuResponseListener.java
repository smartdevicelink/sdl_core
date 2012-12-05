package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.DeleteSubMenuResponse;

public interface ISyncDeleteSubMenuResponseListener {
	public void onDeleteSubMenuResponse(DeleteSubMenuResponse response, Object tag);
}