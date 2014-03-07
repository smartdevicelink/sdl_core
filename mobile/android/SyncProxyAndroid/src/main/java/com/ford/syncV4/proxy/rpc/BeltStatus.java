package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataEventStatus;
import com.ford.syncV4.util.DebugTool;

public class BeltStatus extends RPCStruct {

    public BeltStatus() { }
    public BeltStatus(Hashtable hash) {
        super(hash);
    }

    public void setDriverBeltDeployed(VehicleDataEventStatus driverBeltDeployed) {
        if (driverBeltDeployed != null) {
            store.put(Names.driverBeltDeployed, driverBeltDeployed);
        } else {
        	store.remove(Names.driverBeltDeployed);
        }
    }
    public VehicleDataEventStatus getDriverBeltDeployed() {
        Object obj = store.get(Names.driverBeltDeployed);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.driverBeltDeployed, e);
            }
            return theCode;
        }
        return null;
    }
    public void setPassengerBeltDeployed(VehicleDataEventStatus passengerBeltDeployed) {
        if (passengerBeltDeployed != null) {
            store.put(Names.passengerBeltDeployed, passengerBeltDeployed);
        } else {
        	store.remove(Names.passengerBeltDeployed);
        }
    }
    public VehicleDataEventStatus getPassengerBeltDeployed() {
        Object obj = store.get(Names.passengerBeltDeployed);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.passengerBeltDeployed, e);
            }
            return theCode;
        }
        return null;
    }
    public void setPassengerBuckleBelted(VehicleDataEventStatus passengerBuckleBelted) {
        if (passengerBuckleBelted != null) {
            store.put(Names.passengerBuckleBelted, passengerBuckleBelted);
        } else {
        	store.remove(Names.passengerBuckleBelted);
        }
    }
    public VehicleDataEventStatus getPassengerBuckleBelted() {
        Object obj = store.get(Names.passengerBuckleBelted);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.passengerBuckleBelted, e);
            }
            return theCode;
        }
        return null;
    }
    public void setDriverBuckleBelted(VehicleDataEventStatus driverBuckleBelted) {
        if (driverBuckleBelted != null) {
            store.put(Names.driverBuckleBelted, driverBuckleBelted);
        } else {
        	store.remove(Names.driverBuckleBelted);
        }
    }
    public VehicleDataEventStatus getDriverBuckleBelted() {
        Object obj = store.get(Names.driverBuckleBelted);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.driverBuckleBelted, e);
            }
            return theCode;
        }
        return null;
    }
    public void setLeftRow2BuckleBelted(VehicleDataEventStatus leftRow2BuckleBelted) {
        if (leftRow2BuckleBelted != null) {
            store.put(Names.leftRow2BuckleBelted, leftRow2BuckleBelted);
        } else {
        	store.remove(Names.leftRow2BuckleBelted);
        }
    }
    public VehicleDataEventStatus getLeftRow2BuckleBelted() {
        Object obj = store.get(Names.leftRow2BuckleBelted);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.leftRow2BuckleBelted, e);
            }
            return theCode;
        }
        return null;
    }
    public void setPassengerChildDetected(VehicleDataEventStatus passengerChildDetected) {
        if (passengerChildDetected != null) {
            store.put(Names.passengerChildDetected, passengerChildDetected);
        } else {
        	store.remove(Names.passengerChildDetected);
        }
    }
    public VehicleDataEventStatus getPassengerChildDetected() {
        Object obj = store.get(Names.passengerChildDetected);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.passengerChildDetected, e);
            }
            return theCode;
        }
        return null;
    }
    public void setRightRow2BuckleBelted(VehicleDataEventStatus rightRow2BuckleBelted) {
        if (rightRow2BuckleBelted != null) {
            store.put(Names.rightRow2BuckleBelted, rightRow2BuckleBelted);
        } else {
        	store.remove(Names.rightRow2BuckleBelted);
        }
    }
    public VehicleDataEventStatus getRightRow2BuckleBelted() {
        Object obj = store.get(Names.rightRow2BuckleBelted);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.rightRow2BuckleBelted, e);
            }
            return theCode;
        }
        return null;
    }
    public void setMiddleRow2BuckleBelted(VehicleDataEventStatus middleRow2BuckleBelted) {
        if (middleRow2BuckleBelted != null) {
            store.put(Names.middleRow2BuckleBelted, middleRow2BuckleBelted);
        } else {
        	store.remove(Names.middleRow2BuckleBelted);
        }
    }
    public VehicleDataEventStatus getMiddleRow2BuckleBelted() {
        Object obj = store.get(Names.middleRow2BuckleBelted);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.middleRow2BuckleBelted, e);
            }
            return theCode;
        }
        return null;
    }
    public void setMiddleRow3BuckleBelted(VehicleDataEventStatus middleRow3BuckleBelted) {
        if (middleRow3BuckleBelted != null) {
            store.put(Names.middleRow3BuckleBelted, middleRow3BuckleBelted);
        } else {
        	store.remove(Names.middleRow3BuckleBelted);
        }
    }
    public VehicleDataEventStatus getMiddleRow3BuckleBelted() {
        Object obj = store.get(Names.middleRow3BuckleBelted);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.middleRow3BuckleBelted, e);
            }
            return theCode;
        }
        return null;
    }
    public void setLeftRow3BuckleBelted(VehicleDataEventStatus leftRow3BuckleBelted) {
        if (leftRow3BuckleBelted != null) {
            store.put(Names.leftRow3BuckleBelted, leftRow3BuckleBelted);
        } else {
        	store.remove(Names.leftRow3BuckleBelted);
        }
    }
    public VehicleDataEventStatus getLeftRow3BuckleBelted() {
        Object obj = store.get(Names.leftRow3BuckleBelted);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.leftRow3BuckleBelted, e);
            }
            return theCode;
        }
        return null;
    }
    public void setRightRow3BuckleBelted(VehicleDataEventStatus rightRow3BuckleBelted) {
        if (rightRow3BuckleBelted != null) {
            store.put(Names.rightRow3BuckleBelted, rightRow3BuckleBelted);
        } else {
        	store.remove(Names.rightRow3BuckleBelted);
        }
    }
    public VehicleDataEventStatus getRightRow3BuckleBelted() {
        Object obj = store.get(Names.rightRow3BuckleBelted);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.rightRow3BuckleBelted, e);
            }
            return theCode;
        }
        return null;
    }
    public void setLeftRearInflatableBelted(VehicleDataEventStatus rearInflatableBelted) {
        if (rearInflatableBelted != null) {
            store.put(Names.rearInflatableBelted, rearInflatableBelted);
        } else {
        	store.remove(Names.rearInflatableBelted);
        }
    }
    public VehicleDataEventStatus getLeftRearInflatableBelted() {
        Object obj = store.get(Names.rearInflatableBelted);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.rearInflatableBelted, e);
            }
            return theCode;
        }
        return null;
    }
    public void setRightRearInflatableBelted(VehicleDataEventStatus rightRearInflatableBelted) {
        if (rightRearInflatableBelted != null) {
            store.put(Names.rightRearInflatableBelted, rightRearInflatableBelted);
        } else {
        	store.remove(Names.rightRearInflatableBelted);
        }
    }
    public VehicleDataEventStatus getRightRearInflatableBelted() {
        Object obj = store.get(Names.rightRearInflatableBelted);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.rightRearInflatableBelted, e);
            }
            return theCode;
        }
        return null;
    }
    public void setMiddleRow1BeltDeployed(VehicleDataEventStatus middleRow1BeltDeployed) {
        if (middleRow1BeltDeployed != null) {
            store.put(Names.middleRow1BeltDeployed, middleRow1BeltDeployed);
        } else {
        	store.remove(Names.middleRow1BeltDeployed);
        }
    }
    public VehicleDataEventStatus getMiddleRow1BeltDeployed() {
        Object obj = store.get(Names.middleRow1BeltDeployed);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.middleRow1BeltDeployed, e);
            }
            return theCode;
        }
        return null;
    }
    public void setMiddleRow1BuckleBelted(VehicleDataEventStatus middleRow1BuckleBelted) {
        if (middleRow1BuckleBelted != null) {
            store.put(Names.middleRow1BuckleBelted, middleRow1BuckleBelted);
        } else {
        	store.remove(Names.middleRow1BuckleBelted);
        }
    }
    public VehicleDataEventStatus getMiddleRow1BuckleBelted() {
        Object obj = store.get(Names.middleRow1BuckleBelted);
        if (obj instanceof VehicleDataEventStatus) {
            return (VehicleDataEventStatus) obj;
        } else if (obj instanceof String) {
        	VehicleDataEventStatus theCode = null;
            try {
                theCode = VehicleDataEventStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.middleRow1BuckleBelted, e);
            }
            return theCode;
        }
        return null;
    }
}