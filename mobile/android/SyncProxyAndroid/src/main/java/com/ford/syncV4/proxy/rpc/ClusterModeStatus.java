package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.CarModeStatus;
import com.ford.syncV4.proxy.rpc.enums.PowerModeQualificationStatus;
import com.ford.syncV4.proxy.rpc.enums.PowerModeStatus;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataNotificationStatus;
import com.ford.syncV4.util.DebugTool;

public class ClusterModeStatus extends RPCStruct {

    public ClusterModeStatus() { }
    public ClusterModeStatus(Hashtable hash) {
        super(hash);
    }

    public void setPowerModeActive(Boolean powerModeActive) {
        if (powerModeActive != null) {
        	store.put(Names.powerModeActive, powerModeActive);
        } else {
        	store.remove(Names.powerModeActive);
        }
    }
    public Boolean getPowerModeActive() {
        return (Boolean) store.get(Names.powerModeActive);
    }
    public void setPowerModeQualificationStatus(PowerModeQualificationStatus powerModeQualificationStatus) {
        if (powerModeQualificationStatus != null) {
            store.put(Names.powerModeQualificationStatus, powerModeQualificationStatus);
        } else {
        	store.remove(Names.powerModeQualificationStatus);
        }
    }
    public PowerModeQualificationStatus getPowerModeQualificationStatus() {
        Object obj = store.get(Names.powerModeQualificationStatus);
        if (obj instanceof PowerModeQualificationStatus) {
            return (PowerModeQualificationStatus) obj;
        } else if (obj instanceof String) {
        	PowerModeQualificationStatus theCode = null;
            try {
                theCode = PowerModeQualificationStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.powerModeQualificationStatus, e);
            }
            return theCode;
        }
        return null;
    }
    public void setCarModeStatus(CarModeStatus carModeStatus) {
        if (carModeStatus != null) {
            store.put(Names.carModeStatus, carModeStatus);
        } else {
        	store.remove(Names.carModeStatus);
        }
    }
    public CarModeStatus getCarModeStatus() {
        Object obj = store.get(Names.carModeStatus);
        if (obj instanceof CarModeStatus) {
            return (CarModeStatus) obj;
        } else if (obj instanceof String) {
        	CarModeStatus theCode = null;
            try {
                theCode = CarModeStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.carModeStatus, e);
            }
            return theCode;
        }
        return null;
    }
    public void setPowerModeStatus(PowerModeStatus powerModeStatus) {
        if (powerModeStatus != null) {
            store.put(Names.powerModeStatus, powerModeStatus);
        } else {
        	store.remove(Names.powerModeStatus);
        }
    }
    public PowerModeStatus getPowerModeStatus() {
        Object obj = store.get(Names.powerModeStatus);
        if (obj instanceof PowerModeStatus) {
            return (PowerModeStatus) obj;
        } else if (obj instanceof String) {
        	PowerModeStatus theCode = null;
            try {
                theCode = PowerModeStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.powerModeStatus, e);
            }
            return theCode;
        }
        return null;
    }
}