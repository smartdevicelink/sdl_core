package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataResultCode;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataType;

public class VehicleDataResult extends RPCStruct {

    public VehicleDataResult() { }
    public VehicleDataResult(Hashtable hash) {
        super(hash);
    }
    public void setDataType(VehicleDataType dataType) {
    	if (dataType != null) {
    		store.put(Names.dataType, dataType);
    	} else {
    		store.remove(Names.dataType);
    	}
    }
    public VehicleDataType getDataType() {
    	return (VehicleDataType) store.get(Names.dataType);
    }
    public void setResultCode(VehicleDataResultCode resultCode) {
    	if (resultCode != null) {
    		store.put(Names.resultCode, resultCode);
    	} else {
    		store.remove(Names.resultCode);
    	}
    }
    public VehicleDataResultCode getResultCode() {
    	return (VehicleDataResultCode) store.get(Names.resultCode);
    }
}
