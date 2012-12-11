package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;

public class SubscribeVehicleDataResponse extends RPCResponse {

    public SubscribeVehicleDataResponse() {
        super("SubscribeVehicleData");
    }
    public SubscribeVehicleDataResponse(Hashtable hash) {
        super(hash);
    }
    public void setDataResult(VehicleDataResult dataResult) {
    	if (dataResult != null) {
    		parameters.put(Names.dataResult, dataResult);
    	} else {
    		parameters.remove(Names.dataResult);
    	}
    }
    public VehicleDataResult getDataResult() {
    	return (VehicleDataResult) parameters.get(Names.dataResult);
    }
}