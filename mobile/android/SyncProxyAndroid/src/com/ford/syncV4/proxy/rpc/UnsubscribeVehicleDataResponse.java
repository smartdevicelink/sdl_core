package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataType;
import com.ford.syncV4.util.DebugTool;

public class UnsubscribeVehicleDataResponse extends RPCResponse {

    public UnsubscribeVehicleDataResponse() {
        super("UnsubscribeVehicleData");
    }
    public UnsubscribeVehicleDataResponse(Hashtable hash) {
        super(hash);
    }
    public void setGps(VehicleDataResult gps) {
        if (gps != null) {
            parameters.put(Names.gps, gps);
        } else {
        	parameters.remove(Names.gps);
        }
    }
    public VehicleDataResult getGps() {
    	Object obj = parameters.get(Names.gps);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.gps, e);
            }
        }
        return null;
    }
    public void setSpeed(VehicleDataResult speed) {
        if (speed != null) {
            parameters.put(Names.speed, speed);
        } else {
        	parameters.remove(Names.speed);
        }
    }
    public VehicleDataResult getSpeed() {
    	Object obj = parameters.get(Names.speed);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.speed, e);
            }
        }
        return null;
    }
    public void setRpm(VehicleDataResult rpm) {
        if (rpm != null) {
            parameters.put(Names.rpm, rpm);
        } else {
        	parameters.remove(Names.rpm);
        }
    }
    public VehicleDataResult getRpm() {
    	Object obj = parameters.get(Names.rpm);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.rpm, e);
            }
        }
        return null;
    }
    public void setFuelLevel(VehicleDataResult fuelLevel) {
        if (fuelLevel != null) {
            parameters.put(Names.fuelLevel, fuelLevel);
        } else {
        	parameters.remove(Names.fuelLevel);
        }
    }
    public VehicleDataResult getFuelLevel() {
    	Object obj = parameters.get(Names.fuelLevel);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.fuelLevel, e);
            }
        }
        return null;
    }
    public void setFuelLevel_State(VehicleDataResult fuelLevel_State) {
        if (fuelLevel_State != null) {
            parameters.put(Names.fuelLevel_State, fuelLevel_State);
        } else {
        	parameters.remove(Names.fuelLevel_State);
        }
    }
    public VehicleDataResult getFuelLevel_State() {
    	Object obj = parameters.get(Names.fuelLevel_State);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.fuelLevel_State, e);
            }
        }
        return null;
    }
    public void setInstantFuelConsumption(VehicleDataResult instantFuelConsumption) {
        if (instantFuelConsumption != null) {
            parameters.put(Names.instantFuelConsumption, instantFuelConsumption);
        } else {
        	parameters.remove(Names.instantFuelConsumption);
        }
    }
    public VehicleDataResult getInstantFuelConsumption() {
    	Object obj = parameters.get(Names.instantFuelConsumption);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.instantFuelConsumption, e);
            }
        }
        return null;
    }
    public void setExternalTemperature(VehicleDataResult externalTemperature) {
        if (externalTemperature != null) {
            parameters.put(Names.externalTemperature, externalTemperature);
        } else {
        	parameters.remove(Names.externalTemperature);
        }
    }
    public VehicleDataResult getExternalTemperature() {
    	Object obj = parameters.get(Names.externalTemperature);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.externalTemperature, e);
            }
        }
        return null;
    }
    public void setPrndl(VehicleDataResult prndl) {
        if (prndl != null) {
            parameters.put(Names.prndl, prndl);
        } else {
        	parameters.remove(Names.prndl);
        }
    }
    public VehicleDataResult getPrndl() {
    	Object obj = parameters.get(Names.prndl);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.prndl, e);
            }
        }
        return null;
    }
    public void setTirePressure(VehicleDataResult tirePressure) {
        if (tirePressure != null) {
            parameters.put(Names.tirePressure, tirePressure);
        } else {
        	parameters.remove(Names.tirePressure);
        }
    }
    public VehicleDataResult getTirePressure() {
    	Object obj = parameters.get(Names.tirePressure);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.tirePressure, e);
            }
        }
        return null;
    }
    public void setOdometer(VehicleDataResult odometer) {
        if (odometer != null) {
            parameters.put(Names.odometer, odometer);
        } else {
        	parameters.remove(Names.odometer);
        }
    }
    public VehicleDataResult getOdometer() {
    	Object obj = parameters.get(Names.odometer);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.odometer, e);
            }
        }
        return null;
    }
    public void setBeltStatus(VehicleDataResult beltStatus) {
        if (beltStatus != null) {
            parameters.put(Names.beltStatus, beltStatus);
        } else {
        	parameters.remove(Names.beltStatus);
        }
    }
    public VehicleDataResult getBeltStatus() {
    	Object obj = parameters.get(Names.beltStatus);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.beltStatus, e);
            }
        }
        return null;
    }
    public void setBodyInformation(VehicleDataResult bodyInformation) {
        if (bodyInformation != null) {
            parameters.put(Names.bodyInformation, bodyInformation);
        } else {
        	parameters.remove(Names.bodyInformation);
        }
    }
    public VehicleDataResult getBodyInformation() {
    	Object obj = parameters.get(Names.bodyInformation);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.bodyInformation, e);
            }
        }
        return null;
    }
    public void setDeviceStatus(VehicleDataResult deviceStatus) {
        if (deviceStatus != null) {
            parameters.put(Names.deviceStatus, deviceStatus);
        } else {
        	parameters.remove(Names.deviceStatus);
        }
    }
    public VehicleDataResult getDeviceStatus() {
    	Object obj = parameters.get(Names.deviceStatus);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.deviceStatus, e);
            }
        }
        return null;
    }
    public void setDriverBraking(VehicleDataResult driverBraking) {
        if (driverBraking != null) {
            parameters.put(Names.driverBraking, driverBraking);
        } else {
        	parameters.remove(Names.driverBraking);
        }
    }
    public VehicleDataResult getDriverBraking() {
    	Object obj = parameters.get(Names.driverBraking);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.driverBraking, e);
            }
        }
        return null;
    }
    public void setWiperStatus(VehicleDataResult wiperStatus) {
        if (wiperStatus != null) {
            parameters.put(Names.wiperStatus, wiperStatus);
        } else {
        	parameters.remove(Names.wiperStatus);
        }
    }
    public VehicleDataResult getWiperStatus() {
    	Object obj = parameters.get(Names.wiperStatus);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.wiperStatus, e);
            }
        }
        return null;
    }
    public void setFuelEconomy(VehicleDataResult fuelEconomy) {
        if (fuelEconomy != null) {
            parameters.put(Names.fuelEconomy, fuelEconomy);
        } else {
        	parameters.remove(Names.fuelEconomy);
        }
    }
    public VehicleDataResult getFuelEconomy() {
    	Object obj = parameters.get(Names.fuelEconomy);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.fuelEconomy, e);
            }
        }
        return null;
    }
    public void setEngineOilLife(VehicleDataResult engineOilLife) {
        if (engineOilLife != null) {
            parameters.put(Names.engineOilLife, engineOilLife);
        } else {
        	parameters.remove(Names.engineOilLife);
        }
    }
    public VehicleDataResult getEngineOilLife() {
    	Object obj = parameters.get(Names.engineOilLife);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.engineOilLife, e);
            }
        }
        return null;
    }
    public void setHeadLampStatus(VehicleDataResult headLampStatus) {
        if (headLampStatus != null) {
            parameters.put(Names.headLampStatus, headLampStatus);
        } else {
        	parameters.remove(Names.headLampStatus);
        }
    }
    public VehicleDataResult getHeadLampStatus() {
    	Object obj = parameters.get(Names.headLampStatus);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.headLampStatus, e);
            }
        }
        return null;
    }
    public void setBatteryVoltage(VehicleDataResult batteryVoltage) {
        if (batteryVoltage != null) {
            parameters.put(Names.batteryVoltage, batteryVoltage);
        } else {
        	parameters.remove(Names.batteryVoltage);
        }
    }
    public VehicleDataResult getBatteryVoltage() {
    	Object obj = parameters.get(Names.batteryVoltage);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.batteryVoltage, e);
            }
        }
        return null;
    }
    public void setBrakeTorque(VehicleDataResult brakeTorque) {
        if (brakeTorque != null) {
            parameters.put(Names.brakeTorque, brakeTorque);
        } else {
        	parameters.remove(Names.brakeTorque);
        }
    }
    public VehicleDataResult getBrakeTorque() {
    	Object obj = parameters.get(Names.brakeTorque);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.brakeTorque, e);
            }
        }
        return null;
    }
    public void setEngineTorque(VehicleDataResult engineTorque) {
        if (engineTorque != null) {
            parameters.put(Names.engineTorque, engineTorque);
        } else {
        	parameters.remove(Names.engineTorque);
        }
    }
    public VehicleDataResult getEngineTorque() {
    	Object obj = parameters.get(Names.engineTorque);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.engineTorque, e);
            }
        }
        return null;
    }
    public void setTurboBoost(VehicleDataResult turboBoost) {
        if (turboBoost != null) {
            parameters.put(Names.turboBoost, turboBoost);
        } else {
        	parameters.remove(Names.turboBoost);
        }
    }
    public VehicleDataResult getTurboBoost() {
    	Object obj = parameters.get(Names.turboBoost);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.turboBoost, e);
            }
        }
        return null;
    }
    public void setCoolantTemp(VehicleDataResult coolantTemp) {
        if (coolantTemp != null) {
            parameters.put(Names.coolantTemp, coolantTemp);
        } else {
        	parameters.remove(Names.coolantTemp);
        }
    }
    public VehicleDataResult getCoolantTemp() {
    	Object obj = parameters.get(Names.coolantTemp);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.coolantTemp, e);
            }
        }
        return null;
    }
    public void setAirFuelRatio(VehicleDataResult airFuelRatio) {
        if (airFuelRatio != null) {
            parameters.put(Names.airFuelRatio, airFuelRatio);
        } else {
        	parameters.remove(Names.airFuelRatio);
        }
    }
    public VehicleDataResult getAirFuelRatio() {
    	Object obj = parameters.get(Names.airFuelRatio);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.airFuelRatio, e);
            }
        }
        return null;
    }
    public void setCoolingHeadTemp(VehicleDataResult coolingHeadTemp) {
        if (coolingHeadTemp != null) {
            parameters.put(Names.coolingHeadTemp, coolingHeadTemp);
        } else {
        	parameters.remove(Names.coolingHeadTemp);
        }
    }
    public VehicleDataResult getCoolingHeadTemp() {
    	Object obj = parameters.get(Names.coolingHeadTemp);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.coolingHeadTemp, e);
            }
        }
        return null;
    }
    public void setOilTemp(VehicleDataResult oilTemp) {
        if (oilTemp != null) {
            parameters.put(Names.oilTemp, oilTemp);
        } else {
        	parameters.remove(Names.oilTemp);
        }
    }
    public VehicleDataResult getOilTemp() {
    	Object obj = parameters.get(Names.oilTemp);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.oilTemp, e);
            }
        }
        return null;
    }
    public void setIntakeAirTemp(VehicleDataResult intakeAirTemp) {
        if (intakeAirTemp != null) {
            parameters.put(Names.intakeAirTemp, intakeAirTemp);
        } else {
        	parameters.remove(Names.intakeAirTemp);
        }
    }
    public VehicleDataResult getIntakeAirTemp() {
    	Object obj = parameters.get(Names.intakeAirTemp);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.intakeAirTemp, e);
            }
        }
        return null;
    }
    public void setGearShiftAdvice(VehicleDataResult gearShiftAdvice) {
        if (gearShiftAdvice != null) {
            parameters.put(Names.gearShiftAdvice, gearShiftAdvice);
        } else {
        	parameters.remove(Names.gearShiftAdvice);
        }
    }
    public VehicleDataResult getGearShiftAdvice() {
    	Object obj = parameters.get(Names.gearShiftAdvice);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.gearShiftAdvice, e);
            }
        }
        return null;
    }
    public void setAcceleration(VehicleDataResult acceleration) {
        if (acceleration != null) {
            parameters.put(Names.acceleration, acceleration);
        } else {
        	parameters.remove(Names.acceleration);
        }
    }
    public VehicleDataResult getAcceleration() {
    	Object obj = parameters.get(Names.acceleration);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.acceleration, e);
            }
        }
        return null;
    }
    public void setAccPedalPosition(VehicleDataResult accPedalPosition) {
        if (accPedalPosition != null) {
            parameters.put(Names.accPedalPosition, accPedalPosition);
        } else {
        	parameters.remove(Names.accPedalPosition);
        }
    }
    public VehicleDataResult getAccPedalPosition() {
    	Object obj = parameters.get(Names.accPedalPosition);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.accPedalPosition, e);
            }
        }
        return null;
    }
    public void setClutchPedalPosition(VehicleDataResult clutchPedalPosition) {
        if (clutchPedalPosition != null) {
            parameters.put(Names.clutchPedalPosition, clutchPedalPosition);
        } else {
        	parameters.remove(Names.clutchPedalPosition);
        }
    }
    public VehicleDataResult getClutchPedalPosition() {
    	Object obj = parameters.get(Names.clutchPedalPosition);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.clutchPedalPosition, e);
            }
        }
        return null;
    }
    public void setReverseGearStatus(VehicleDataResult reverseGearStatus) {
        if (reverseGearStatus != null) {
            parameters.put(Names.reverseGearStatus, reverseGearStatus);
        } else {
        	parameters.remove(Names.reverseGearStatus);
        }
    }
    public VehicleDataResult getReverseGearStatus() {
    	Object obj = parameters.get(Names.reverseGearStatus);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.reverseGearStatus, e);
            }
        }
        return null;
    }
    public void setAccTorque(VehicleDataResult accTorque) {
        if (accTorque != null) {
            parameters.put(Names.accTorque, accTorque);
        } else {
        	parameters.remove(Names.accTorque);
        }
    }
    public VehicleDataResult getAccTorque() {
    	Object obj = parameters.get(Names.accTorque);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.accTorque, e);
            }
        }
        return null;
    }
    public void setEvInfo(VehicleDataResult evInfo) {
        if (evInfo != null) {
            parameters.put(Names.evInfo, evInfo);
        } else {
        	parameters.remove(Names.evInfo);
        }
    }
    public VehicleDataResult getEvInfo() {
    	Object obj = parameters.get(Names.evInfo);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.evInfo, e);
            }
        }
        return null;
    }
    public void setAmbientLightStatus(VehicleDataResult ambientLightStatus) {
        if (ambientLightStatus != null) {
            parameters.put(Names.ambientLightStatus, ambientLightStatus);
        } else {
        	parameters.remove(Names.ambientLightStatus);
        }
    }
    public VehicleDataResult getAmbientLightStatus() {
    	Object obj = parameters.get(Names.ambientLightStatus);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.ambientLightStatus, e);
            }
        }
        return null;
    }
    public void setECallInfo(VehicleDataResult eCallInfo) {
        if (eCallInfo != null) {
            parameters.put(Names.eCallInfo, eCallInfo);
        } else {
        	parameters.remove(Names.eCallInfo);
        }
    }
    public VehicleDataResult getECallInfo() {
    	Object obj = parameters.get(Names.eCallInfo);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.eCallInfo, e);
            }
        }
        return null;
    }
    public void setAirbagStatus(VehicleDataResult airbagStatus) {
        if (airbagStatus != null) {
            parameters.put(Names.airbagStatus, airbagStatus);
        } else {
        	parameters.remove(Names.airbagStatus);
        }
    }
    public VehicleDataResult getAirbagStatus() {
    	Object obj = parameters.get(Names.airbagStatus);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.airbagStatus, e);
            }
        }
        return null;
    }
    public void setEmergencyEvent(VehicleDataResult emergencyEvent) {
        if (emergencyEvent != null) {
            parameters.put(Names.emergencyEvent, emergencyEvent);
        } else {
        	parameters.remove(Names.emergencyEvent);
        }
    }
    public VehicleDataResult getEmergencyEvent() {
    	Object obj = parameters.get(Names.emergencyEvent);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.emergencyEvent, e);
            }
        }
        return null;
    }
    public void setClusterModeStatus(VehicleDataResult clusterModeStatus) {
        if (clusterModeStatus != null) {
            parameters.put(Names.clusterModeStatus, clusterModeStatus);
        } else {
        	parameters.remove(Names.clusterModeStatus);
        }
    }
    public VehicleDataResult getClusterModeStatus() {
    	Object obj = parameters.get(Names.clusterModeStatus);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.clusterModeStatus, e);
            }
        }
        return null;
    }
    public void setMyKey(VehicleDataResult myKey) {
        if (myKey != null) {
            parameters.put(Names.myKey, myKey);
        } else {
        	parameters.remove(Names.myKey);
        }
    }
    public VehicleDataResult getMyKey() {
    	Object obj = parameters.get(Names.myKey);
        if (obj instanceof VehicleDataResult) {
            return (VehicleDataResult) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new VehicleDataResult((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.myKey, e);
            }
        }
        return null;
    }
}
