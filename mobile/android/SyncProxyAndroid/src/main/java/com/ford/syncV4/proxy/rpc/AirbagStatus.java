package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataEventStatus;
import com.ford.syncV4.util.DebugTool;

public class AirbagStatus extends RPCStruct {

    public AirbagStatus() { }
    public AirbagStatus(Hashtable hash) {
        super(hash);
    }

    public void setDriverAirbagDeployed(VehicleDataEventStatus driverAirbagDeployed) {
        if (driverAirbagDeployed != null) {
            store.put(Names.driverAirbagDeployed, driverAirbagDeployed);
        } else {
        	store.remove(Names.driverAirbagDeployed);
        }
    }
    public VehicleDataEventStatus getDriverAirbagDeployed() {
        Object obj = store.get(Names.driverAirbagDeployed);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.driverAirbagDeployed, e);
            }
            return theCode;
        }
        return null;
    }
    public void setDriverSideAirbagDeployed(VehicleDataEventStatus driverSideAirbagDeployed) {
        if (driverSideAirbagDeployed != null) {
            store.put(Names.driverSideAirbagDeployed, driverSideAirbagDeployed);
        } else {
        	store.remove(Names.driverSideAirbagDeployed);
        }
    }
    public VehicleDataEventStatus getDriverSideAirbagDeployed() {
        Object obj = store.get(Names.driverSideAirbagDeployed);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.driverSideAirbagDeployed, e);
            }
            return theCode;
        }
        return null;
    }
    public void setDriverCurtainAirbagDeployed(VehicleDataEventStatus driverCurtainAirbagDeployed) {
        if (driverCurtainAirbagDeployed != null) {
            store.put(Names.driverCurtainAirbagDeployed, driverCurtainAirbagDeployed);
        } else {
        	store.remove(Names.driverCurtainAirbagDeployed);
        }
    }
    public VehicleDataEventStatus getDriverCurtainAirbagDeployed() {
        Object obj = store.get(Names.driverCurtainAirbagDeployed);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.driverCurtainAirbagDeployed, e);
            }
            return theCode;
        }
        return null;
    }
    public void setPassengerAirbagDeployed(VehicleDataEventStatus passengerAirbagDeployed) {
        if (passengerAirbagDeployed != null) {
            store.put(Names.passengerAirbagDeployed, passengerAirbagDeployed);
        } else {
        	store.remove(Names.passengerAirbagDeployed);
        }
    }
    public VehicleDataEventStatus getPassengerAirbagDeployed() {
        Object obj = store.get(Names.passengerAirbagDeployed);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.passengerAirbagDeployed, e);
            }
            return theCode;
        }
        return null;
    }
    public void setPassengerCurtainAirbagDeployed(VehicleDataEventStatus passengerCurtainAirbagDeployed) {
        if (passengerCurtainAirbagDeployed != null) {
            store.put(Names.passengerCurtainAirbagDeployed, passengerCurtainAirbagDeployed);
        } else {
        	store.remove(Names.passengerCurtainAirbagDeployed);
        }
    }
    public VehicleDataEventStatus getPassengerCurtainAirbagDeployed() {
        Object obj = store.get(Names.passengerCurtainAirbagDeployed);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.passengerCurtainAirbagDeployed, e);
            }
            return theCode;
        }
        return null;
    }
    public void setDriverKneeAirbagDeployed(VehicleDataEventStatus driverKneeAirbagDeployed) {
        if (driverKneeAirbagDeployed != null) {
            store.put(Names.driverKneeAirbagDeployed, driverKneeAirbagDeployed);
        } else {
        	store.remove(Names.driverKneeAirbagDeployed);
        }
    }
    public VehicleDataEventStatus getDriverKneeAirbagDeployed() {
        Object obj = store.get(Names.driverKneeAirbagDeployed);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.driverKneeAirbagDeployed, e);
            }
            return theCode;
        }
        return null;
    }
    public void setPassengerSideAirbagDeployed(VehicleDataEventStatus passengerSideAirbagDeployed) {
        if (passengerSideAirbagDeployed != null) {
            store.put(Names.passengerSideAirbagDeployed, passengerSideAirbagDeployed);
        } else {
        	store.remove(Names.passengerSideAirbagDeployed);
        }
    }
    public VehicleDataEventStatus getPassengerSideAirbagDeployed() {
        Object obj = store.get(Names.passengerSideAirbagDeployed);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.passengerSideAirbagDeployed, e);
            }
            return theCode;
        }
        return null;
    }
    public void setPassengerKneeAirbagDeployed(VehicleDataEventStatus passengerKneeAirbagDeployed) {
        if (passengerKneeAirbagDeployed != null) {
            store.put(Names.passengerKneeAirbagDeployed, passengerKneeAirbagDeployed);
        } else {
        	store.remove(Names.passengerKneeAirbagDeployed);
        }
    }
    public VehicleDataEventStatus getPassengerKneeAirbagDeployed() {
        Object obj = store.get(Names.passengerKneeAirbagDeployed);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.passengerKneeAirbagDeployed, e);
            }
            return theCode;
        }
        return null;
    }
}