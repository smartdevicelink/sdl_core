package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class EncodedSyncPData extends RPCRequest {
	public EncodedSyncPData() {
        super("EncodedSyncPData");
    }
    
    public EncodedSyncPData(Hashtable hash) {
        super(hash);
    }
    
    public Vector<String> getData() {
        if (parameters.get(Names.data) instanceof Vector<?>) {
        	Vector<?> list = (Vector<?>)parameters.get(Names.data);
        	if (list != null && list.size()>0) {
        		Object obj = list.get(0);
        		if (obj instanceof String) {
        			return (Vector<String>) list;
        		}
        	}
        }
    	return null;
    }
    public void setData( Vector<String> data ) {
    	if ( data!= null) {
    		parameters.put(Names.data, data );
    	}
    }
}
