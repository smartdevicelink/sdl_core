/**
 *
 */
package com.ford.syncV4.proxy;

import java.util.Hashtable;

import com.ford.syncV4.proxy.constants.Names;

public class RPCRequest extends RPCMessage {

    /**
     * Indicates whether this request going to be encrypted or not
     */
    private boolean doEncryption = false;
	public RPCRequest(String functionName) {
		super(functionName, Names.request);
		messageType = Names.request;
	}

    public RPCRequest(String functionName) {
        super(functionName, Names.request);
        messageType = Names.request;
    }

    public RPCRequest(Hashtable hash) {
        super(hash);
    }

    public RPCRequest(RPCRequest that) {
        // FIXME test this!
        super(that);
        this.setCorrelationID(that.getCorrelationID());
    }

    public Integer getCorrelationID() {
        return (Integer) function.get(Names.correlationID);
    }

    public void setCorrelationID(Integer correlationID) {
        if (correlationID != null) {
            function.put(Names.correlationID, correlationID);
        } else if (parameters.contains(Names.correlationID)) {
            function.remove(Names.correlationID);
        }
    }

    /**
     * @return whether or not this request going to be encrypted when sending to SDL
     */
    public boolean isDoEncryption() {
        return doEncryption;
    }

    /**
     * Set whether or not this request going to be encrypted when sending to SDL
     * @param doEncryption boolean value
     */
    public void setDoEncryption(boolean doEncryption) {
        this.doEncryption = doEncryption;
    }
}