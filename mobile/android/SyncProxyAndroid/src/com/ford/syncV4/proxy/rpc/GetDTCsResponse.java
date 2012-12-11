package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.HMILevel;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataResultCode;
import com.ford.syncV4.util.DebugTool;

public class GetDTCsResponse extends RPCResponse {

    public GetDTCsResponse() {
        super("GetDTCs");
    }
    public GetDTCsResponse(Hashtable hash) {
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
    	if (parameters.get(Names.dataResult) instanceof Vector<?>) {
	        Vector<?> list = (Vector<?>)parameters.get(Names.dataResult);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof VehicleDataResultCode) {
	                return (Vector<VehicleDataResultCode>) list;
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
    	return (Vector<String>) parameters.get(Names.data);
    }
}