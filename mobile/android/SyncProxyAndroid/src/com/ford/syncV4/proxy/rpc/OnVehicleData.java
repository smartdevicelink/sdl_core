package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.PRNDL;
import com.ford.syncV4.util.DebugTool;

public class OnVehicleData extends RPCNotification {

    public OnVehicleData() {
        super("OnVehicleData");
    }
    public OnVehicleData(Hashtable hash) {
        super(hash);
    }
    public void setGps(GPSData gps) {
    	if (gps != null) {
    		parameters.put(Names.gps, gps);
    	} else {
    		parameters.remove(Names.gps);
    	}
    }
    public GPSData getGps() {
    	Object obj = parameters.get(Names.gps);
        if (obj instanceof GPSData) {
            return (GPSData) obj;
        } else {
        	GPSData theCode = null;
            try {
                theCode = new GPSData((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.gps, e);
            }
            return theCode;
        }
    }
    public void setSpeed(Integer speed) {
    	if (speed != null) {
    		store.put(Names.speed, speed);
    	} else {
    		store.remove(Names.speed);
    	}
    }
    public Integer getSpeed() {
    	return (Integer) store.get(Names.speed);
    }
    public void setRpm(Integer rpm) {
    	if (rpm != null) {
    		store.put(Names.rpm, rpm);
    	} else {
    		store.remove(Names.rpm);
    	}
    }
    public Integer getRpm() {
    	return (Integer) store.get(Names.rpm);
    }
    public void setFuelLevel(Float fuelLevel) {
    	if (fuelLevel != null) {
    		store.put(Names.fuelLevel, fuelLevel);
    	} else {
    		store.remove(Names.fuelLevel);
    	}
    }
    public Float getFuelLevel() {
    	return (Float) store.get(Names.fuelLevel);
    }
    public void setAvgFuelEconomy(Float avgFuelEconomy) {
    	if (avgFuelEconomy != null) {
    		store.put(Names.avgFuelEconomy, avgFuelEconomy);
    	} else {
    		store.remove(Names.avgFuelEconomy);
    	}
    }
    public Float getAvgFuelEconomy() {
    	return (Float) store.get(Names.avgFuelEconomy);
    }
    public void setBatteryVoltage(Float batteryVoltage) {
    	if (batteryVoltage != null) {
    		store.put(Names.batteryVoltage, batteryVoltage);
    	} else {
    		store.remove(Names.batteryVoltage);
    	}
    }
    public Float getBatteryVoltage() {
    	return (Float) store.get(Names.batteryVoltage);
    }
    public void setExternalTemperature(Float externalTemperature) {
    	if (externalTemperature != null) {
    		store.put(Names.externalTemperature, externalTemperature);
    	} else {
    		store.remove(Names.externalTemperature);
    	}
    }
    public Float getExternalTemperature() {
    	return (Float) store.get(Names.externalTemperature);
    }
    public void setVin(String vin) {
    	if (vin != null) {
    		store.put(Names.vin, vin);
    	} else {
    		store.remove(Names.vin);
    	}
    }
    public String getVin() {
    	return (String) store.get(Names.vin);
    }
    public void setPrndl(PRNDL prndl) {
    	if (prndl != null) {
    		store.put(Names.prndl, prndl);
    	} else {
    		store.remove(Names.prndl);
    	}
    }
    public PRNDL getPrndl() {
        Object obj = store.get(Names.prndl);
        if (obj instanceof PRNDL) {
            return (PRNDL) obj;
        } else if (obj instanceof String) {
        	PRNDL theCode = null;
            try {
                theCode = PRNDL.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.prndl, e);
            }
            return theCode;
        }
        return null;
    }
    public void setTirePressure(TireStatus tirePressure) {
    	if (tirePressure != null) {
    		store.put(Names.tirePressure, tirePressure);
    	} else {
    		store.remove(Names.tirePressure);
    	}
    }
    public TireStatus getTirePressure() {
    	Object obj = store.get(Names.tirePressure);
        if (obj instanceof TireStatus) {
            return (TireStatus) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new TireStatus((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.tirePressure, e);
            }
        }
        return null;
    }
    public void setBatteryPackVoltage(Float batteryPackVoltage) {
    	if (batteryPackVoltage != null) {
    		store.put(Names.batteryPackVoltage, batteryPackVoltage);
    	} else {
    		store.remove(Names.batteryPackVoltage);
    	}
    }
    public Float getBatteryPackVoltage() {
    	return (Float) store.get(Names.batteryPackVoltage);
    }
    public void setBatteryPackCurrent(Float batteryPackCurrent) {
    	if (batteryPackCurrent != null) {
    		store.put(Names.batteryPackCurrent, batteryPackCurrent);
    	} else {
    		store.remove(Names.batteryPackCurrent);
    	}
    }
    public Float getBatteryPackCurrent() {
    	return (Float) store.get(Names.batteryPackCurrent);
    }
    public void setBatteryPackTemperature(Integer batteryPackTemperature) {
    	if (batteryPackTemperature != null) {
    		store.put(Names.batteryPackTemperature, batteryPackTemperature);
    	} else {
    		store.remove(Names.batteryPackTemperature);
    	}
    }
    public Integer getBatteryPackTemperature() {
    	return (Integer) store.get(Names.batteryPackTemperature);
    }

    public void setHeadLampStatus(HeadLampStatus headLampStatus) {
        if (headLampStatus != null) {
            parameters.put(Names.headLampStatus, headLampStatus);
        } else {
            parameters.remove(Names.headLampStatus);
        }
    }

    public HeadLampStatus getHeadLampStatus() {
        Object obj = parameters.get(Names.headLampStatus);
        if (obj instanceof HeadLampStatus) {
            return (HeadLampStatus) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new HeadLampStatus((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.headLampStatus, e);
            }
        }
        return null;
    }

    public void setEngineTorque(Integer engineTorque) {
    	if (engineTorque != null) {
    		store.put(Names.engineTorque, engineTorque);
    	} else {
    		store.remove(Names.engineTorque);
    	}
    }
    public Integer getEngineTorque() {
    	return (Integer) store.get(Names.engineTorque);
    }
    public void setOdometer(Integer odometer) {
    	if (odometer != null) {
    		store.put(Names.odometer, odometer);
    	} else {
    		store.remove(Names.odometer);
    	}
    }
    public Integer getOdometer() {
    	return (Integer) store.get(Names.odometer);
    }
    public void setTripOdometer(Integer tripOdometer) {
    	if (tripOdometer != null) {
    		store.put(Names.tripOdometer, tripOdometer);
    	} else {
    		store.remove(Names.tripOdometer);
    	}
    }
    public Integer getTripOdometer() {
    	return (Integer) store.get(Names.tripOdometer);
    }
    public void setSatRadioESN(String satRadioESN) {
    	if (satRadioESN != null) {
    		store.put(Names.satRadioESN, satRadioESN);
    	} else {
    		store.remove(Names.satRadioESN);
    	}
    }
    public String getSatRadioESN() {
    	return (String) store.get(Names.satRadioESN);
    }

    public void setSteeringWheelAngle(Float steeringWheelAngle) {
        if (steeringWheelAngle != null) {
            parameters.put(Names.steeringWheelAngle, steeringWheelAngle);
        } else {
            parameters.remove(Names.steeringWheelAngle);
        }
    }

    public Float getSteeringWheelAngle() {
        final Object o = parameters.get(Names.steeringWheelAngle);
        if (o instanceof Float) {
            return (Float) o;
        }
        if (o instanceof Double) {
            return ((Double) o).floatValue();
        }
        return null;
    }

    public void setECallInfo(ECallInfo eCallInfo) {
        if (eCallInfo != null) {
            parameters.put(Names.eCallInfo, eCallInfo);
        } else {
            parameters.remove(Names.eCallInfo);
        }
    }

    public ECallInfo getECallInfo() {
        Object obj = parameters.get(Names.eCallInfo);
        if (obj instanceof ECallInfo) {
            return (ECallInfo) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new ECallInfo((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.eCallInfo, e);
            }
        }
        return null;
    }

    public void setAirbagStatus(AirbagStatus airbagStatus) {
        if (airbagStatus != null) {
            parameters.put(Names.airbagStatus, airbagStatus);
        } else {
            parameters.remove(Names.airbagStatus);
        }
    }

    public AirbagStatus getAirbagStatus() {
        Object obj = parameters.get(Names.airbagStatus);
        if (obj instanceof AirbagStatus) {
            return (AirbagStatus) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new AirbagStatus((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.airbagStatus, e);
            }
        }
        return null;
    }

    public void setEmergencyEvent(EmergencyEvent emergencyEvent) {
        if (emergencyEvent != null) {
            parameters.put(Names.emergencyEvent, emergencyEvent);
        } else {
            parameters.remove(Names.emergencyEvent);
        }
    }

    public EmergencyEvent getEmergencyEvent() {
        Object obj = parameters.get(Names.emergencyEvent);
        if (obj instanceof EmergencyEvent) {
            return (EmergencyEvent) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new EmergencyEvent((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.emergencyEvent, e);
            }
        }
        return null;
    }

    public void setClusterModeStatus(ClusterModeStatus clusterModeStatus) {
        if (clusterModeStatus != null) {
            parameters.put(Names.clusterModeStatus, clusterModeStatus);
        } else {
            parameters.remove(Names.clusterModeStatus);
        }
    }

    public ClusterModeStatus getClusterModeStatus() {
        Object obj = parameters.get(Names.clusterModeStatus);
        if (obj instanceof ClusterModeStatus) {
            return (ClusterModeStatus) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new ClusterModeStatus((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.clusterModeStatus, e);
            }
        }
        return null;
    }

    public void setMyKey(MyKey myKey) {
        if (myKey != null) {
            parameters.put(Names.myKey, myKey);
        } else {
            parameters.remove(Names.myKey);
        }
    }

    public MyKey getMyKey() {
        Object obj = parameters.get(Names.myKey);
        if (obj instanceof MyKey) {
            return (MyKey) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new MyKey((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.myKey, e);
            }
        }
        return null;
    }
}