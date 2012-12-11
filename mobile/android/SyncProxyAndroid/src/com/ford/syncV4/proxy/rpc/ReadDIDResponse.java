package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataResultCode;

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
    	return (Vector<VehicleDataResultCode>) parameters.get(Names.dataResult);
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