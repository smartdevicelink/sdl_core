package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.ECallConfirmationStatus;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataNotificationStatus;
import com.ford.syncV4.util.DebugTool;

public class ECallInfo extends RPCStruct {

    public ECallInfo() { }
    public ECallInfo(Hashtable hash) {
        super(hash);
    }

    public void setECallNotificationStatus(VehicleDataNotificationStatus eCallNotificationStatus) {
        if (eCallNotificationStatus != null) {
            store.put(Names.eCallNotificationStatus, eCallNotificationStatus);
        } else {
        	store.remove(Names.eCallNotificationStatus);
        }
    }
    public VehicleDataNotificationStatus getECallNotificationStatus() {
        Object obj = store.get(Names.eCallNotificationStatus);
        if (obj instanceof VehicleDataNotificationStatus) {
            return (VehicleDataNotificationStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataNotificationStatus theCode = null;
            try {
                theCode = VehicleDataNotificationStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.eCallNotificationStatus, e);
            }
            return theCode;
        }
        return null;
    }
    public void setAuxECallNotificationStatus(VehicleDataNotificationStatus auxECallNotificationStatus) {
        if (auxECallNotificationStatus != null) {
            store.put(Names.auxECallNotificationStatus, auxECallNotificationStatus);
        } else {
        	store.remove(Names.auxECallNotificationStatus);
        }
    }
    public VehicleDataNotificationStatus getAuxECallNotificationStatus() {
        Object obj = store.get(Names.auxECallNotificationStatus);
        if (obj instanceof VehicleDataNotificationStatus) {
            return (VehicleDataNotificationStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataNotificationStatus theCode = null;
            try {
                theCode = VehicleDataNotificationStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.auxECallNotificationStatus, e);
            }
            return theCode;
        }
        return null;
    }
    public void setECallConfirmationStatus(ECallConfirmationStatus eCallConfirmationStatus) {
        if (eCallConfirmationStatus != null) {
            store.put(Names.eCallConfirmationStatus, eCallConfirmationStatus);
        } else {
        	store.remove(Names.eCallConfirmationStatus);
        }
    }
    public ECallConfirmationStatus getECallConfirmationStatus() {
        Object obj = store.get(Names.eCallConfirmationStatus);
        if (obj instanceof ECallConfirmationStatus) {
            return (ECallConfirmationStatus) obj;
        } else if (obj instanceof String) {
        	ECallConfirmationStatus theCode = null;
            try {
                theCode = ECallConfirmationStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.eCallConfirmationStatus, e);
            }
            return theCode;
        }
        return null;
    }
}