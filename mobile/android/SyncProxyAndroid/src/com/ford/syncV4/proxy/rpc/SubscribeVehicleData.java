package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataType;
import com.ford.syncV4.util.DebugTool;

public class SubscribeVehicleData extends RPCRequest {

    public SubscribeVehicleData() {
        super("SubscribeVehicleData");
    }
    public SubscribeVehicleData(Hashtable hash) {
        super(hash);
    }
    public void setDataType(Vector<VehicleDataType> dataType) {
    	if (dataType != null) {
    		parameters.put(Names.dataType, dataType);
    	} else {
    		parameters.remove(Names.dataType);
    	}
    }
    public Vector<VehicleDataType> getDataType() {
        if (parameters.get(Names.dataType) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.dataType);
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
	                    	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.dataType, e);
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
