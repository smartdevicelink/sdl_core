package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.HMILevel;
import com.ford.syncV4.util.DebugTool;

public class HMIPermissions extends RPCStruct {

    public HMIPermissions() { }
    public HMIPermissions(Hashtable hash) {
        super(hash);
    }
    public Vector<HMILevel> getAllowed() {
        if (store.get(Names.allowed) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)store.get(Names.allowed);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof HMILevel) {
	                return (Vector<HMILevel>) list;
	            } else if (obj instanceof String) {
	                Vector<HMILevel> newList = new Vector<HMILevel>();
	                for (Object hashObj : list) {
	                    String strFormat = (String)hashObj;
	                    HMILevel toAdd = null;
	                    try {
	                        toAdd = HMILevel.valueForString(strFormat);
	                    } catch (Exception e) {
	                    	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.allowed, e);
	                    }
	                    if (toAdd != null) {
	                        newList.add(toAdd);
	                    }
	                }
	                return newList;
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
    public Vector<HMILevel> getUserDisallowed() {
        if (store.get(Names.userDisallowed) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)store.get(Names.userDisallowed);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof HMILevel) {
	                return (Vector<HMILevel>) list;
	            } else if (obj instanceof String) {
	                Vector<HMILevel> newList = new Vector<HMILevel>();
	                for (Object hashObj : list) {
	                    String strFormat = (String)hashObj;
	                    HMILevel toAdd = null;
	                    try {
	                        toAdd = HMILevel.valueForString(strFormat);
	                    } catch (Exception e) {
	                    	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.userDisallowed, e);
	                    }
	                    if (toAdd != null) {
	                        newList.add(toAdd);
	                    }
	                }
	                return newList;
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
