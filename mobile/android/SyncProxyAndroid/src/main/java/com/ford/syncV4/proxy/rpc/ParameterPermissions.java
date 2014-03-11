package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.HMILevel;

public class ParameterPermissions extends RPCStruct {

    public ParameterPermissions() { }
    public ParameterPermissions(Hashtable hash) {
        super(hash);
    }
    public Vector<String> getAllowed() {
        if (store.get(Names.allowed) instanceof Vector<?>) {
        	Vector<?> list = (Vector<?>)store.get( Names.allowed);
        	if (list != null && list.size() > 0) {
        		Object obj = list.get(0);
        		if (obj instanceof String) {
                	return (Vector<String>) list;        			
        		}
        	}
        }
        return null;
    }
    public void setAllowed(HMILevel allowed) {
        if (allowed != null) {
            store.put(Names.allowed, allowed);
        } else {
    		store.remove(Names.allowed);
    	}
    }
    public Vector<String> getUserDisallowed() {
        if (store.get(Names.userDisallowed) instanceof Vector<?>) {
        	Vector<?> list = (Vector<?>)store.get( Names.userDisallowed);
        	if (list != null && list.size() > 0) {
        		Object obj = list.get(0);
        		if (obj instanceof String) {
                	return (Vector<String>) list;        			
        		}
        	}
        }
        return null;
    }
    public void setUserDisallowed(HMILevel userDisallowed) {
        if (userDisallowed != null) {
            store.put(Names.userDisallowed, userDisallowed);
        } else {
    		store.remove(Names.userDisallowed);
    	}
    }
}
