package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataType;
import com.ford.syncV4.util.DebugTool;

public class UnsubscribeVehicleData extends RPCRequest {

    public UnsubscribeVehicleData() {
        super("UnsubscribeVehicleData");
    }
    public UnsubscribeVehicleData(Hashtable hash) {
        super(hash);
    }
    public void setGps(Boolean gps) {
        if (gps != null) {
            parameters.put(Names.gps, gps);
        } else {
        	parameters.remove(Names.gps);
        }
    }
    public Boolean getGps() {
        return (Boolean) parameters.get(Names.gps);
    }
    public void setSpeed(Boolean speed) {
        if (speed != null) {
            parameters.put(Names.speed, speed);
        } else {
        	parameters.remove(Names.speed);
        }
    }
    public Boolean getSpeed() {
        return (Boolean) parameters.get(Names.speed);
    }
    public void setRpm(Boolean rpm) {
        if (rpm != null) {
            parameters.put(Names.rpm, rpm);
        } else {
        	parameters.remove(Names.rpm);
        }
    }
    public Boolean getRpm() {
        return (Boolean) parameters.get(Names.rpm);
    }
    public void setFuelLevel(Boolean fuelLevel) {
        if (fuelLevel != null) {
            parameters.put(Names.fuelLevel, fuelLevel);
        } else {
        	parameters.remove(Names.fuelLevel);
        }
    }
    public Boolean getFuelLevel() {
        return (Boolean) parameters.get(Names.fuelLevel);
    }
    public void setFuelLevel_State(Boolean fuelLevel_State) {
        if (fuelLevel_State != null) {
            parameters.put(Names.fuelLevel_State, fuelLevel_State);
        } else {
        	parameters.remove(Names.fuelLevel_State);
        }
    }
    public Boolean getFuelLevel_State() {
        return (Boolean) parameters.get(Names.fuelLevel_State);
    }
    public void setInstantFuelConsumption(Boolean instantFuelConsumption) {
        if (instantFuelConsumption != null) {
            parameters.put(Names.instantFuelConsumption, instantFuelConsumption);
        } else {
        	parameters.remove(Names.instantFuelConsumption);
        }
    }
    public Boolean getInstantFuelConsumption() {
        return (Boolean) parameters.get(Names.instantFuelConsumption);
    }
    public void setExternalTemperature(Boolean externalTemperature) {
        if (externalTemperature != null) {
            parameters.put(Names.externalTemperature, externalTemperature);
        } else {
        	parameters.remove(Names.externalTemperature);
        }
    }
    public Boolean getExternalTemperature() {
        return (Boolean) parameters.get(Names.externalTemperature);
    }
    public void setPrndl(Boolean prndl) {
        if (prndl != null) {
            parameters.put(Names.prndl, prndl);
        } else {
        	parameters.remove(Names.prndl);
        }
    }
    public Boolean getPrndl() {
        return (Boolean) parameters.get(Names.prndl);
    }
    public void setTirePressure(Boolean tirePressure) {
        if (tirePressure != null) {
            parameters.put(Names.tirePressure, tirePressure);
        } else {
        	parameters.remove(Names.tirePressure);
        }
    }
    public Boolean getTirePressure() {
        return (Boolean) parameters.get(Names.tirePressure);
    }
    public void setOdometer(Boolean odometer) {
        if (odometer != null) {
            parameters.put(Names.odometer, odometer);
        } else {
        	parameters.remove(Names.odometer);
        }
    }
    public Boolean getOdometer() {
        return (Boolean) parameters.get(Names.odometer);
    }
    public void setBeltStatus(Boolean beltStatus) {
        if (beltStatus != null) {
            parameters.put(Names.beltStatus, beltStatus);
        } else {
        	parameters.remove(Names.beltStatus);
        }
    }
    public Boolean getBeltStatus() {
        return (Boolean) parameters.get(Names.beltStatus);
    }
    public void setBodyInformation(Boolean bodyInformation) {
        if (bodyInformation != null) {
            parameters.put(Names.bodyInformation, bodyInformation);
        } else {
        	parameters.remove(Names.bodyInformation);
        }
    }
    public Boolean getBodyInformation() {
        return (Boolean) parameters.get(Names.bodyInformation);
    }
    public void setDeviceStatus(Boolean deviceStatus) {
        if (deviceStatus != null) {
            parameters.put(Names.deviceStatus, deviceStatus);
        } else {
        	parameters.remove(Names.deviceStatus);
        }
    }
    public Boolean getDeviceStatus() {
        return (Boolean) parameters.get(Names.deviceStatus);
    }
    public void setDriverBraking(Boolean driverBraking) {
        if (driverBraking != null) {
            parameters.put(Names.driverBraking, driverBraking);
        } else {
        	parameters.remove(Names.driverBraking);
        }
    }
    public Boolean getDriverBraking() {
        return (Boolean) parameters.get(Names.driverBraking);
    }
    public void setWiperStatus(Boolean wiperStatus) {
        if (wiperStatus != null) {
            parameters.put(Names.wiperStatus, wiperStatus);
        } else {
        	parameters.remove(Names.wiperStatus);
        }
    }
    public Boolean getWiperStatus() {
        return (Boolean) parameters.get(Names.wiperStatus);
    }
    public void setHeadLampStatus(Boolean headLampStatus) {
        if (headLampStatus != null) {
            parameters.put(Names.headLampStatus, headLampStatus);
        } else {
        	parameters.remove(Names.headLampStatus);
        }
    }
    public Boolean getHeadLampStatus() {
        return (Boolean) parameters.get(Names.headLampStatus);
    }
    public void setBatteryVoltage(Boolean batteryVoltage) {
        if (batteryVoltage != null) {
            parameters.put(Names.batteryVoltage, batteryVoltage);
        } else {
        	parameters.remove(Names.batteryVoltage);
        }
    }
    public Boolean getBatteryVoltage() {
        return (Boolean) parameters.get(Names.batteryVoltage);
    }
    public void setEngineTorque(Boolean engineTorque) {
        if (engineTorque != null) {
            parameters.put(Names.engineTorque, engineTorque);
        } else {
        	parameters.remove(Names.engineTorque);
        }
    }
    public Boolean getEngineTorque() {
        return (Boolean) parameters.get(Names.engineTorque);
    }
    public void setAccPedalPosition(Boolean accPedalPosition) {
        if (accPedalPosition != null) {
            parameters.put(Names.accPedalPosition, accPedalPosition);
        } else {
        	parameters.remove(Names.accPedalPosition);
        }
    }
    public Boolean getAccPedalPosition() {
        return (Boolean) parameters.get(Names.accPedalPosition);
    }

    public void setSteeringWheelAngle(Boolean steeringWheelAngle) {
        if (steeringWheelAngle != null) {
            parameters.put(Names.steeringWheelAngle, steeringWheelAngle);
        } else {
            parameters.remove(Names.steeringWheelAngle);
        }
    }

    public Boolean getSteeringWheelAngle() {
        final Object o = parameters.get(Names.steeringWheelAngle);
        if (o instanceof Boolean) {
            return (Boolean) o;
        }
        return null;
    }

    public void setECallInfo(Boolean eCallInfo) {
        if (eCallInfo != null) {
            parameters.put(Names.eCallInfo, eCallInfo);
        } else {
        	parameters.remove(Names.eCallInfo);
        }
    }
    public Boolean getECallInfo() {
        return (Boolean) parameters.get(Names.eCallInfo);
    }
    public void setAirbagStatus(Boolean airbagStatus) {
        if (airbagStatus != null) {
            parameters.put(Names.airbagStatus, airbagStatus);
        } else {
        	parameters.remove(Names.airbagStatus);
        }
    }
    public Boolean getAirbagStatus() {
        return (Boolean) parameters.get(Names.airbagStatus);
    }
    public void setEmergencyEvent(Boolean emergencyEvent) {
        if (emergencyEvent != null) {
            parameters.put(Names.emergencyEvent, emergencyEvent);
        } else {
        	parameters.remove(Names.emergencyEvent);
        }
    }
    public Boolean getEmergencyEvent() {
        return (Boolean) parameters.get(Names.emergencyEvent);
    }
    public void setClusterModeStatus(Boolean clusterModeStatus) {
        if (clusterModeStatus != null) {
            parameters.put(Names.clusterModeStatus, clusterModeStatus);
        } else {
        	parameters.remove(Names.clusterModeStatus);
        }
    }
    public Boolean getClusterModeStatus() {
        return (Boolean) parameters.get(Names.clusterModeStatus);
    }
    public void setMyKey(Boolean myKey) {
        if (myKey != null) {
            parameters.put(Names.myKey, myKey);
        } else {
        	parameters.remove(Names.myKey);
        }
    }
    public Boolean getMyKey() {
        return (Boolean) parameters.get(Names.myKey);
    }
}
