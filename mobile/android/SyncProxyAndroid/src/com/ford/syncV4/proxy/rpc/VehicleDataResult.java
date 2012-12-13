package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.DisplayType;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataResultCode;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataType;
import com.ford.syncV4.util.DebugTool;

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
        Object obj = store.get(Names.dataType);
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
    public void setResultCode(VehicleDataResultCode resultCode) {
    	if (resultCode != null) {
    		store.put(Names.resultCode, resultCode);
    	} else {
    		store.remove(Names.resultCode);
    	}
    }
    public VehicleDataResultCode getResultCode() {
        Object obj = store.get(Names.resultCode);
        if (obj instanceof VehicleDataResultCode) {
            return (VehicleDataResultCode) obj;
        } else if (obj instanceof String) {
        	VehicleDataResultCode theCode = null;
            try {
                theCode = VehicleDataResultCode.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.resultCode, e);
            }
            return theCode;
        }
        return null;
    }
}
