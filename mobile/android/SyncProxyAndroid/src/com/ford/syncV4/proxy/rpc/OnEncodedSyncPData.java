package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;


import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.constants.Names;

public class OnEncodedSyncPData extends RPCNotification {
	public OnEncodedSyncPData() {
        super("OnEncodedSyncPData");
    }
    public OnEncodedSyncPData(Hashtable hash) {
        super(hash);
    }
    
    public Vector<String> getData() {
    	if (parameters.get(Names.data) instanceof Vector<?>) {
    		Vector<?> list = (Vector<?>)parameters.get(Names.data);
    		if (list != null && list.size()>0) {
        		Object obj = list.get(0);
        		if (obj instanceof String) {
        			return (Vector<String>)list;
        		}
    		}
    	}
        return null;
    }
    
    public void setData( Vector<String> data ) {
        if (data != null) {
            parameters.put(Names.data, data );
        }
    }
    
    public String getUrl(){
    	if (parameters.get(Names.URL) instanceof String) {
    		return (String)parameters.get(Names.URL);
    	}
    	return null;
    }
    
    public void setUrl( String url ) {
    	if (url != null) {
    		parameters.put(Names.URL, url);
    	}
    }
}
