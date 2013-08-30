package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.IgnitionStableStatus;
import com.ford.syncV4.proxy.rpc.enums.IgnitionStatus;
import com.ford.syncV4.util.DebugTool;

public class BodyInformation extends RPCStruct {

    public BodyInformation() { }
    public BodyInformation(Hashtable hash) {
        super(hash);
    }

    public void setParkBrakeActive(Boolean parkBrakeActive) {
        if (parkBrakeActive != null) {
        	store.put(Names.parkBrakeActive, parkBrakeActive);
        } else {
        	store.remove(Names.parkBrakeActive);
        }
    }
    public Boolean getParkBrakeActive() {
        return (Boolean) store.get(Names.parkBrakeActive);
    }
    public void setIgnitionStableStatus(IgnitionStableStatus ignitionStableStatus) {
        if (ignitionStableStatus != null) {
            store.put(Names.ignitionStableStatus, ignitionStableStatus);
        } else {
        	store.remove(Names.ignitionStableStatus);
        }
    }
    public IgnitionStableStatus getIgnitionStableStatus() {
        Object obj = store.get(Names.ignitionStableStatus);
        if (obj instanceof IgnitionStableStatus) {
            return (IgnitionStableStatus) obj;
        } else if (obj instanceof String) {
        	IgnitionStableStatus theCode = null;
            try {
                theCode = IgnitionStableStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.ignitionStableStatus, e);
            }
            return theCode;
        }
        return null;
    }
    public void setIgnitionStatus(IgnitionStatus ignitionStatus) {
        if (ignitionStatus != null) {
            store.put(Names.ignitionStatus, ignitionStatus);
        } else {
        	store.remove(Names.ignitionStatus);
        }
    }
    public IgnitionStatus getIgnitionStatus() {
        Object obj = store.get(Names.ignitionStatus);
        if (obj instanceof IgnitionStatus) {
            return (IgnitionStatus) obj;
        } else if (obj instanceof String) {
        	IgnitionStatus theCode = null;
            try {
                theCode = IgnitionStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.ignitionStatus, e);
            }
            return theCode;
        }
        return null;
    }
}