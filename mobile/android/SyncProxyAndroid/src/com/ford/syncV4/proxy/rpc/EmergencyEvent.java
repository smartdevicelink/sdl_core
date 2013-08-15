package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.EmergencyEventType;
import com.ford.syncV4.proxy.rpc.enums.FuelCutoffStatus;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataEventStatus;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataNotificationStatus;
import com.ford.syncV4.util.DebugTool;

public class EmergencyEvent extends RPCStruct {

    public EmergencyEvent() { }
    public EmergencyEvent(Hashtable hash) {
        super(hash);
    }

    public void setEmergencyEventType(EmergencyEventType emergencyEventType) {
        if (emergencyEventType != null) {
            store.put(Names.emergencyEventType, emergencyEventType);
        } else {
        	store.remove(Names.emergencyEventType);
        }
    }
    public EmergencyEventType getEmergencyEventType() {
        Object obj = store.get(Names.emergencyEventType);
        if (obj instanceof EmergencyEventType) {
            return (EmergencyEventType) obj;
        } else if (obj instanceof String) {
        	EmergencyEventType theCode = null;
            try {
                theCode = EmergencyEventType.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.emergencyEventType, e);
            }
            return theCode;
        }
        return null;
    }
    public void setFuelCutoffStatus(FuelCutoffStatus fuelCutoffStatus) {
        if (fuelCutoffStatus != null) {
            store.put(Names.fuelCutoffStatus, fuelCutoffStatus);
        } else {
        	store.remove(Names.fuelCutoffStatus);
        }
    }
    public FuelCutoffStatus getFuelCutoffStatus() {
        Object obj = store.get(Names.fuelCutoffStatus);
        if (obj instanceof FuelCutoffStatus) {
            return (FuelCutoffStatus) obj;
        } else if (obj instanceof String) {
        	FuelCutoffStatus theCode = null;
            try {
                theCode = FuelCutoffStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.fuelCutoffStatus, e);
            }
            return theCode;
        }
        return null;
    }
    public void setRolloverEvent(VehicleDataEventStatus rolloverEvent) {
        if (rolloverEvent != null) {
            store.put(Names.rolloverEvent, rolloverEvent);
        } else {
        	store.remove(Names.rolloverEvent);
        }
    }
    public VehicleDataEventStatus getRolloverEvent() {
        Object obj = store.get(Names.rolloverEvent);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.rolloverEvent, e);
            }
            return theCode;
        }
        return null;
    }
    public void setMaximumChangeVelocity(VehicleDataEventStatus maximumChangeVelocity) {
        if (maximumChangeVelocity != null) {
            store.put(Names.maximumChangeVelocity, maximumChangeVelocity);
        } else {
        	store.remove(Names.maximumChangeVelocity);
        }
    }
    public VehicleDataEventStatus getMaximumChangeVelocity() {
        Object obj = store.get(Names.maximumChangeVelocity);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.maximumChangeVelocity, e);
            }
            return theCode;
        }
        return null;
    }
    public void setMultipleEvents(VehicleDataEventStatus multipleEvents) {
        if (multipleEvents != null) {
            store.put(Names.multipleEvents, multipleEvents);
        } else {
        	store.remove(Names.multipleEvents);
        }
    }
    public VehicleDataEventStatus getMultipleEvents() {
        Object obj = store.get(Names.multipleEvents);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.multipleEvents, e);
            }
            return theCode;
        }
        return null;
    }
}