package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;

public class DTC extends RPCStruct {

    public DTC() { }
    public DTC(Hashtable hash) {
        super(hash);
    }
    public void setIdentifier(String identifier) {
    	if (identifier != null) {
    		store.put(Names.identifier, identifier);
    	} else {
    		store.remove(Names.identifier);
    	}
    }
    public String getIdentifier() {
    	return (String) store.get(Names.identifier);
    }
    public void setStatusByte(String statusByte) {
    	if (statusByte != null) {
    		store.put(Names.statusByte, statusByte);
    	} else {
    		store.remove(Names.statusByte);
    	}
    }
    public String getStatusByte() {
    	return (String) store.get(Names.statusByte);
    }
}
