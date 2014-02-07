package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.AmbientLightStatus;
import com.ford.syncV4.util.DebugTool;

public class HeadLampStatus extends RPCStruct {

    public HeadLampStatus() {}
    public HeadLampStatus(Hashtable hash) {
        super(hash);
    }

    public void setAmbientLightSensorStatus(
            AmbientLightStatus ambientLightStatus) {
        if (ambientLightStatus != null) {
            store.put(Names.ambientLightSensorStatus, ambientLightStatus);
        } else {
            store.remove(Names.ambientLightSensorStatus);
        }
    }

    public AmbientLightStatus getAmbientLightSensorStatus() {
        Object obj = store.get(Names.ambientLightSensorStatus);
        if (obj instanceof AmbientLightStatus) {
            return (AmbientLightStatus) obj;
        } else if (obj instanceof String) {
            AmbientLightStatus theCode = null;
            try {
                theCode = AmbientLightStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.ambientLightSensorStatus, e);
            }
            return theCode;
        }
        return null;
    }

    public void setHighBeamsOn(Boolean highBeamsOn) {
        if (highBeamsOn != null) {
            store.put(Names.highBeamsOn, highBeamsOn);
        } else {
        	store.remove(Names.highBeamsOn);
        }
    }
    public Boolean getHighBeamsOn() {
    	return (Boolean) store.get(Names.highBeamsOn);
    }
}