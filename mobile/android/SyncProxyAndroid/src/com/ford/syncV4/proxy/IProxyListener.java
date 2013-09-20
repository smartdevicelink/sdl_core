package com.ford.syncV4.proxy;

import com.ford.syncV4.proxy.interfaces.IProxyListenerBase;
import com.ford.syncV4.proxy.rpc.OnAppInterfaceUnregistered;
import com.ford.syncV4.proxy.rpc.RegisterAppInterfaceResponse;
import com.ford.syncV4.proxy.rpc.UnregisterAppInterfaceResponse;

public interface IProxyListener extends IProxyListenerBase{
	// Adds Legacy Life-cycle Management call-backs to the IProxyListenerAbstract interface
	
	public void onProxyOpened();
	
	public void onRegisterAppInterfaceResponse(RegisterAppInterfaceResponse response);

	public void onOnAppInterfaceUnregistered(OnAppInterfaceUnregistered notification);
	
	public void onUnregisterAppInterfaceResponse(UnregisterAppInterfaceResponse response);
	
}
