package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataResultCode;
import com.ford.syncV4.util.DebugTool;

public class ReadDIDResponse extends RPCResponse {

    public ReadDIDResponse() {
        super("ReadDID");
    }
    public ReadDIDResponse(Hashtable hash) {
        super(hash);
    }
    public void setDataResult(Vector<VehicleDataResultCode> dataResult) {
    	if (dataResult != null) {
    		parameters.put(Names.dataResult, dataResult);
    	} else {
    		parameters.remove(Names.dataResult);
    	}
    }
    public Vector<VehicleDataResultCode> getDataResult() {
        if (store.get(Names.dataResult) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)store.get(Names.dataResult);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof VehicleDataResultCode) {
	                return (Vector<VehicleDataResultCode>) list;
	            } else if (obj instanceof String) {
	                Vector<VehicleDataResultCode> newList = new Vector<VehicleDataResultCode>();
	                for (Object hashObj : list) {
	                    String strFormat = (String)hashObj;
	                    VehicleDataResultCode toAdd = null;
	                    try {
	                        toAdd = VehicleDataResultCode.valueForString(strFormat);
	                    } catch (Exception e) {
	                    	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.dataResult, e);
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
    public void setData(Vector<String> data) {
    	if (data != null) {
    		parameters.put(Names.data, data);
    	} else {
    		parameters.remove(Names.data);
    	}
    }
    public Vector<String> getData() {
        if (store.get(Names.data) instanceof Vector<?>) {
        	Vector<?> list = (Vector<?>)store.get(Names.data);
        	if (list != null && list.size() > 0) {
        		Object obj = list.get(0);
        		if (obj instanceof String) {
                	return (Vector<String>) list;        			
        		}
        	}
        }
        return null;
    }
}