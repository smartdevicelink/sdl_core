package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataType;
import com.ford.syncV4.util.DebugTool;

public class SubscribeVehicleData extends RPCRequest {

    public SubscribeVehicleData() {
        super("SubscribeVehicleData");
    }
    public SubscribeVehicleData(Hashtable hash) {
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
    public void setFuelEconomy(Boolean fuelEconomy) {
        if (fuelEconomy != null) {
            parameters.put(Names.fuelEconomy, fuelEconomy);
        } else {
        	parameters.remove(Names.fuelEconomy);
        }
    }
    public Boolean getFuelEconomy() {
        return (Boolean) parameters.get(Names.fuelEconomy);
    }
    public void setEngineOilLife(Boolean engineOilLife) {
        if (engineOilLife != null) {
            parameters.put(Names.engineOilLife, engineOilLife);
        } else {
        	parameters.remove(Names.engineOilLife);
        }
    }
    public Boolean getEngineOilLife() {
        return (Boolean) parameters.get(Names.engineOilLife);
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
    public void setBrakeTorque(Boolean brakeTorque) {
        if (brakeTorque != null) {
            parameters.put(Names.brakeTorque, brakeTorque);
        } else {
        	parameters.remove(Names.brakeTorque);
        }
    }
    public Boolean getBrakeTorque() {
        return (Boolean) parameters.get(Names.brakeTorque);
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
    public void setTurboBoost(Boolean turboBoost) {
        if (turboBoost != null) {
            parameters.put(Names.turboBoost, turboBoost);
        } else {
        	parameters.remove(Names.turboBoost);
        }
    }
    public Boolean getTurboBoost() {
        return (Boolean) parameters.get(Names.turboBoost);
    }
    public void setCoolantTemp(Boolean coolantTemp) {
        if (coolantTemp != null) {
            parameters.put(Names.coolantTemp, coolantTemp);
        } else {
        	parameters.remove(Names.coolantTemp);
        }
    }
    public Boolean getCoolantTemp() {
        return (Boolean) parameters.get(Names.coolantTemp);
    }
    public void setAirFuelRatio(Boolean airFuelRatio) {
        if (airFuelRatio != null) {
            parameters.put(Names.airFuelRatio, airFuelRatio);
        } else {
        	parameters.remove(Names.airFuelRatio);
        }
    }
    public Boolean getAirFuelRatio() {
        return (Boolean) parameters.get(Names.airFuelRatio);
    }
    public void setCoolingHeadTemp(Boolean coolingHeadTemp) {
        if (coolingHeadTemp != null) {
            parameters.put(Names.coolingHeadTemp, coolingHeadTemp);
        } else {
        	parameters.remove(Names.coolingHeadTemp);
        }
    }
    public Boolean getCoolingHeadTemp() {
        return (Boolean) parameters.get(Names.coolingHeadTemp);
    }
    public void setOilTemp(Boolean oilTemp) {
        if (oilTemp != null) {
            parameters.put(Names.oilTemp, oilTemp);
        } else {
        	parameters.remove(Names.oilTemp);
        }
    }
    public Boolean getOilTemp() {
        return (Boolean) parameters.get(Names.oilTemp);
    }
    public void setIntakeAirTemp(Boolean intakeAirTemp) {
        if (intakeAirTemp != null) {
            parameters.put(Names.intakeAirTemp, intakeAirTemp);
        } else {
        	parameters.remove(Names.intakeAirTemp);
        }
    }
    public Boolean getIntakeAirTemp() {
        return (Boolean) parameters.get(Names.intakeAirTemp);
    }
    public void setGearShiftAdvice(Boolean gearShiftAdvice) {
        if (gearShiftAdvice != null) {
            parameters.put(Names.gearShiftAdvice, gearShiftAdvice);
        } else {
        	parameters.remove(Names.gearShiftAdvice);
        }
    }
    public Boolean getGearShiftAdvice() {
        return (Boolean) parameters.get(Names.gearShiftAdvice);
    }
    public void setAcceleration(Boolean acceleration) {
        if (acceleration != null) {
            parameters.put(Names.acceleration, acceleration);
        } else {
        	parameters.remove(Names.acceleration);
        }
    }
    public Boolean getAcceleration() {
        return (Boolean) parameters.get(Names.acceleration);
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
    public void setClutchPedalPosition(Boolean clutchPedalPosition) {
        if (clutchPedalPosition != null) {
            parameters.put(Names.clutchPedalPosition, clutchPedalPosition);
        } else {
        	parameters.remove(Names.clutchPedalPosition);
        }
    }
    public Boolean getClutchPedalPosition() {
        return (Boolean) parameters.get(Names.clutchPedalPosition);
    }
    public void setReverseGearStatus(Boolean reverseGearStatus) {
        if (reverseGearStatus != null) {
            parameters.put(Names.reverseGearStatus, reverseGearStatus);
        } else {
        	parameters.remove(Names.reverseGearStatus);
        }
    }
    public Boolean getReverseGearStatus() {
        return (Boolean) parameters.get(Names.reverseGearStatus);
    }
    public void setAccTorque(Boolean accTorque) {
        if (accTorque != null) {
            parameters.put(Names.accTorque, accTorque);
        } else {
        	parameters.remove(Names.accTorque);
        }
    }
    public Boolean getAccTorque() {
        return (Boolean) parameters.get(Names.accTorque);
    }
    public void setEvInfo(Boolean evInfo) {
        if (evInfo != null) {
            parameters.put(Names.evInfo, evInfo);
        } else {
        	parameters.remove(Names.evInfo);
        }
    }
    public Boolean getEvInfo() {
        return (Boolean) parameters.get(Names.evInfo);
    }
    public void setAmbientLightStatus(Boolean ambientLightStatus) {
        if (ambientLightStatus != null) {
            parameters.put(Names.ambientLightStatus, ambientLightStatus);
        } else {
        	parameters.remove(Names.ambientLightStatus);
        }
    }
    public Boolean getAmbientLightStatus() {
        return (Boolean) parameters.get(Names.ambientLightStatus);
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
