package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.SyncProxyALMManager.SyncSubMenu;
import com.ford.syncV4.proxy.rpc.AddSubMenuResponse;

public interface ISyncAddSubMenuResponseListener {
	public void onAddSubMenuResponse(AddSubMenuResponse response, SyncSubMenu syncSubMenu, Object tag);
}