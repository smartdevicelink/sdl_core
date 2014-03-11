package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.SingleTirePressureStatus;
import com.ford.syncV4.util.DebugTool;

public class SingleTireStatus extends RPCStruct {

    public SingleTireStatus() { }
    public SingleTireStatus(Hashtable hash) {
        super(hash);
    }
    public void setStatus(SingleTirePressureStatus status) {
    	if (status != null) {
    		store.put(Names.status, status);
    	} else {
    		store.remove(Names.status);
    	}
    }
    public SingleTirePressureStatus getStatus() {
        Object obj = store.get(Names.status);
        if (obj instanceof SingleTirePressureStatus) {
            return (SingleTirePressureStatus) obj;
        } else if (obj instanceof String) {
        	SingleTirePressureStatus theCode = null;
            try {
                theCode = SingleTirePressureStatus.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.status, e);
            }
            return theCode;
        }
        return null;
    }
    public void setPressure(Integer pressure) {
    	if (pressure != null) {
    		store.put(Names.pressure, pressure);
    	} else {
    		store.remove(Names.pressure);
    	}
    }
    public Integer getPressure() {
    	return (Integer) store.get(Names.pressure);
    }
}
