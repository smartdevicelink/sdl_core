package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
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
    public void setDataType(VehicleDataType dataType) {
    	if (dataType != null) {
    		parameters.put(Names.dataType, dataType);
    	} else {
    		parameters.remove(Names.dataType);
    	}
    }
    public VehicleDataType getDataType() {
    	Object obj = parameters.get(Names.dataType);
        if (obj instanceof VehicleDataType) {
            return (VehicleDataType) obj;
        } else if (obj instanceof String) {
        	VehicleDataType theCode = null;
            try {
                theCode = VehicleDataType.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.dataType, e);
            }
            return theCode;
        }
        return null;
    }
}
