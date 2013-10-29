package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;

import java.util.Hashtable;
import java.util.Vector;

public class TouchLists extends RPCStruct {
    public TouchLists() {}
    
    public TouchLists(Hashtable hash) {
        super(hash);
    }
    
    public void setTouches(Vector<Integer> touches) {
        if (touches != null) {
            store.put(Names.touches, touches);
        } else {
        	store.remove(Names.touches);
        }
    }
    
    public Vector<Integer> getTouches() {
        if (store.get(Names.touches) instanceof Vector<?>) {
        	Vector<?> list = (Vector<?>)store.get( Names.touches);
        	if (list != null && list.size() > 0) {
        		Object obj = list.get(0);
        		if (obj instanceof String) {
                	return (Vector<Integer>) list;        			
        		}
        	}
        }
        return null;
    }
    
    public void setChangedTouches(Vector<Integer> changedTouches) {
        if (changedTouches != null) {
            store.put(Names.changedTouches, changedTouches);
        } else {
        	store.remove(Names.changedTouches);
        }
    }
    
    public Vector<Integer> getChangedTouches() {
        if (store.get(Names.changedTouches) instanceof Vector<?>) {
        	Vector<?> list = (Vector<?>)store.get( Names.changedTouches);
        	if (list != null && list.size() > 0) {
        		Object obj = list.get(0);
        		if (obj instanceof String) {
                	return (Vector<Integer>) list;        			
        		}
        	}
        }
        return null;
    }
}