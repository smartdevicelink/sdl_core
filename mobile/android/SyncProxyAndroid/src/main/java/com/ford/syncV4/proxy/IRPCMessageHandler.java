package com.ford.syncV4.proxy;

import java.util.Hashtable;

/**
 * Created by Andrew Batutin on 2/10/14
 */
public interface IRPCMessageHandler {

    public void handleRPCMessage(byte sessionId, String appId, Hashtable hash);
}