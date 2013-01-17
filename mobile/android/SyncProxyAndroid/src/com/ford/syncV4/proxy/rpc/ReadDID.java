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
        if (parameters.get(Names.didLocation) instanceof Vector<?>) {
        	Vector<?> list = (Vector<?>)parameters.get(Names.didLocation);
        	if (list != null && list.size() > 0) {
        		Object obj = list.get(0);
        		if (obj instanceof Integer) {
                	return (Vector<Integer>) list;        			
        		}
        	}
        }
        return null;
    }
    public void setEncrypted(Boolean encrypted) {
    	if (encrypted != null) {
    		parameters.put(Names.encrypted, encrypted);
    	} else {
    		parameters.remove(Names.encrypted);
    	}
    }
    public Boolean getEncrypted() {
    	return (Boolean) parameters.get(Names.encrypted);
    }
}
