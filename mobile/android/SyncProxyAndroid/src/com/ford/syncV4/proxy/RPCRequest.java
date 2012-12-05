/**
 * 
 */
package com.ford.syncV4.proxy;

import java.util.Hashtable;

import com.ford.syncV4.proxy.constants.Names;

public class RPCRequest extends RPCMessage {

	public RPCRequest(String functionName) {
		super(functionName, "request");
		messageType = Names.request;
	}

	public RPCRequest(Hashtable hash) {
		super(hash);
	}

	public Integer getCorrelationID() {
		return (Integer)function.get(Names.correlationID);
	}
	
	public void setCorrelationID(Integer correlationID) {
		if (correlationID != null) {
            function.put(Names.correlationID, correlationID );
        } else if (parameters.contains(Names.correlationID)) {
        	function.remove(Names.correlationID);
        }
	}
}
