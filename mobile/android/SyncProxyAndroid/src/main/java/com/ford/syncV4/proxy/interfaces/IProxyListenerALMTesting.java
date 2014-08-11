package com.ford.syncV4.proxy.interfaces;

import com.ford.syncV4.proxy.rpc.RegisterAppInterfaceResponse;
import com.ford.syncV4.proxy.rpc.UnregisterAppInterfaceResponse;

/**
 * IProxyListenerALM interface extended with two callbacks for receiving
 * responses to RegisterAppInterface and UnregisterAppInterface requests.
 */
public interface IProxyListenerALMTesting extends IProxyListenerALM {

	public void onRegisterAppInterfaceResponse(String appId,
                                               RegisterAppInterfaceResponse response);

	public void onUnregisterAppInterfaceResponse(String appId,
                                                 UnregisterAppInterfaceResponse response);

    /**
     * <b>THIS METHOD IS ONLY FOR THE TEST</b><br><br>
     * This is a method which indicates a success states of the Policy workflow in the mobile part
     *
     * @param appId   Application Id
     * @param message Success message
     */
    public void onOnSystemRequestPolicySuccess(String appId, String message);

    /**
     * <b>THIS METHOD IS ONLY FOR THE TEST</b><br><br>
     * This is a method which indicates an error states of the Policy workflow in the mobile part
     *
     * @param appId   Application Id
     * @param message Error message
     */
    public void onOnSystemRequestPolicyError(String appId, String message);
}