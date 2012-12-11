package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class ReadDID extends RPCRequest {

    public ReadDID() {
        super("ReadDID");
    }
    public ReadDID(Hashtable hash) {
        super(hash);
    }
    public void setEcuName(Integer ecuName) {
    	if (ecuName != null) {
    		parameters.put(Names.ecuName, ecuName);
    	} else {
    		parameters.remove(Names.ecuName);
    	}
    }
    public Integer getEcuName() {
    	return (Integer) parameters.get(Names.ecuName);
    }
    public void setDidLocation(Vector<Integer> didLocation) {
    	if (didLocation != null) {
    		parameters.put(Names.didLocation, didLocation);
    	} else {
    		parameters.remove(Names.didLocation);
    	}
    }
    public Vector<Integer> getDidLocation() {
    	return (Vector<Integer>) parameters.get(Names.didLocation);
    }
}
