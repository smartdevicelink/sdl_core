package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.OnSyncChoiceChosen;

public interface ISyncChoiceListener {
	public void onSyncChoiceChosen(OnSyncChoiceChosen notification, Object tag);
}