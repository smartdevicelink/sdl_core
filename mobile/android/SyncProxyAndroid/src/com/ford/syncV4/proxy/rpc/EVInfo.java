package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.MaintenanceModeStatus;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataActiveStatus;
import com.ford.syncV4.util.DebugTool;

public class EVInfo extends RPCStruct {

    public EVInfo() {}
    public EVInfo(Hashtable hash) {
        super(hash);
    }
    public void setElectricFuelConsumption(Float electricFuelConsumption) {
        if (electricFuelConsumption != null) {
        	store.put(Names.electricFuelConsumption, electricFuelConsumption);
        } else {
        	store.remove(Names.electricFuelConsumption);
        }
    }
    public Float getElectricFuelConsumption() {
        return (Float) store.get(Names.electricFuelConsumption);
    }
    public void setStateOfCharge(Float stateOfCharge) {
        if (stateOfCharge != null) {
        	store.put(Names.stateOfCharge, stateOfCharge);
        } else {
        	store.remove(Names.stateOfCharge);
        }
    }
    public Float getStateOfCharge() {
        return (Float) store.get(Names.stateOfCharge);
    }
    public void setFuelMaintenanceMode(MaintenanceModeStatus fuelMaintenanceMode) {
        if (fuelMaintenanceMode != null) {
        	store.put(Names.fuelMaintenanceMode, fuelMaintenanceMode);
        } else {
        	store.remove(Names.fuelMaintenanceMode);
        }
    }
    public MaintenanceModeStatus getFuelMaintenanceMode() {
        Object obj = store.get(Names.fuelMaintenanceMode);
        if (obj instanceof MaintenanceModeStatus) {
            return (MaintenanceModeStatus) obj;
        } else if (obj instanceof String) {
        	MaintenanceModeStatus theCode = null;
            try {
                theCode = MaintenanceModeStatus.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.fuelMaintenanceMode, e);
            }
            return theCode;
        }
        return null;
    }
    public void setDistanceToEmpty(Float distanceToEmpty) {
        if (distanceToEmpty != null) {
        	store.put(Names.distanceToEmpty, distanceToEmpty);
        } else {
        	store.remove(Names.distanceToEmpty);
        }
    }
    public Float getDistanceToEmpty() {
        return (Float) store.get(Names.distanceToEmpty);
    }
}