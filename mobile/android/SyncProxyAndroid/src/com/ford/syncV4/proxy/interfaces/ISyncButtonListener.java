package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.OnButtonEvent;
import com.ford.syncV4.proxy.rpc.OnButtonPress;
import com.ford.syncV4.proxy.rpc.SubscribeButtonResponse;

public interface ISyncButtonListener {
	
	public void onOnButtonEvent(OnButtonEvent notification);

	public void onOnButtonPress(OnButtonPress notification);
	
	public void onSubscribeButtonResponse(SubscribeButtonResponse response, Object tag);
}