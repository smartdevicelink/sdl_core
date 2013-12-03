package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.GlobalProperty;
import com.ford.syncV4.util.DebugTool;

public class ResetGlobalProperties extends RPCRequest {

    public ResetGlobalProperties() {
        super("ResetGlobalProperties");
    }
    public ResetGlobalProperties(Hashtable hash) {
        super(hash);
    }
    public Vector<GlobalProperty> getProperties() {
    	if (parameters.get(Names.properties) instanceof Vector<?>) {
	        Vector<?> list = (Vector<?>)parameters.get(Names.properties);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof GlobalProperty) {
	                return (Vector<GlobalProperty>) list;
	            } else if (obj instanceof String) {
	                Vector<GlobalProperty> newList = new Vector<GlobalProperty>();
	                for (Object hashObj : list) {
	                    String strFormat = (String)hashObj;
	                    GlobalProperty toAdd = null;
	                    try {
	                        toAdd = GlobalProperty.valueForString(strFormat);
	                    } catch (Exception e) {
	                    	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.properties, e);
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
    public void setProperties( Vector<GlobalProperty> properties ) {
        if (properties != null) {
            parameters.put(Names.properties, properties );
        }
    }
}
