package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataType;
import com.ford.syncV4.util.DebugTool;

public class UnsubscribeVehicleDataResponse extends RPCResponse {

    public UnsubscribeVehicleDataResponse() {
        super("UnsubscribeVehicleData");
    }
    public UnsubscribeVehicleDataResponse(Hashtable hash) {
        super(hash);
    }
    public void setDataResult(Vector<VehicleDataType> dataResult) {
    	if (dataResult != null) {
    		parameters.put(Names.dataResult, dataResult);
    	} else {
    		parameters.remove(Names.dataResult);
    	}
    }
    public Vector<VehicleDataType> getDataResult() {
        if (store.get(Names.dataResult) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)store.get(Names.dataResult);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof VehicleDataType) {
	                return (Vector<VehicleDataType>) list;
	            } else if (obj instanceof String) {
	                Vector<VehicleDataType> newList = new Vector<VehicleDataType>();
	                for (Object hashObj : list) {
	                    String strFormat = (String)hashObj;
	                    VehicleDataType toAdd = null;
	                    try {
	                        toAdd = VehicleDataType.valueForString(strFormat);
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
}
