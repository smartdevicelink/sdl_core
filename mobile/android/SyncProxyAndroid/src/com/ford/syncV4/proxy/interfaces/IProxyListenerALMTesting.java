package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.RegisterAppInterfaceResponse;
import com.ford.syncV4.proxy.rpc.UnregisterAppInterfaceResponse;

/**
 * IProxyListenerALM interface extended with two callbacks for receiving
 * responses to RegisterAppInterface and UnregisterAppInterface requests.
 */
public interface IProxyListenerALMTesting extends IProxyListenerALM {
	public void onRegisterAppInterfaceResponse(RegisterAppInterfaceResponse response);
	
	public void onUnregisterAppInterfaceResponse(UnregisterAppInterfaceResponse response);
}
