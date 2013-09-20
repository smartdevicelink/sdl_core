/**
 * 
 */
package com.ford.syncV4.proxy;

import java.util.Hashtable;

public class RPCNotification extends RPCMessage {

	public RPCNotification(String functionName) {
		super(functionName, "notification");
	}

	public RPCNotification(Hashtable hash) {
		super(hash);
	}

	public RPCNotification(RPCMessage rpcMsg) {
		super(rpcMsg);
	}
} // end-class