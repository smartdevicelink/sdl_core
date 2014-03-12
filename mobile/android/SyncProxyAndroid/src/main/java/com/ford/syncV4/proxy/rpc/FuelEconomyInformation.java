package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;

public class FuelEconomyInformation extends RPCStruct {

    public FuelEconomyInformation() { }
    public FuelEconomyInformation(Hashtable hash) {
        super(hash);
    }
    public void setFuelEconomySinceLastReset(Float fuelEconomySinceLastReset) {
        if (fuelEconomySinceLastReset != null) {
        	store.put(Names.fuelEconomySinceLastReset, fuelEconomySinceLastReset);
        } else {
        	store.remove(Names.fuelEconomySinceLastReset);
        }
    }
    public Float getFuelEconomySinceLastReset() {
        return (Float) store.get(Names.fuelEconomySinceLastReset);
    }
    public void setCurrentTripFuelEconomy(Float currentTripFuelEconomy) {
        if (currentTripFuelEconomy != null) {
        	store.put(Names.currentTripFuelEconomy, currentTripFuelEconomy);
        } else {
        	store.remove(Names.currentTripFuelEconomy);
        }
    }
    public Float getCurrentTripFuelEconomy() {
        return (Float) store.get(Names.currentTripFuelEconomy);
    }
    public void setAverageTripFuelEconomy(Float averageTripFuelEconomy) {
        if (averageTripFuelEconomy != null) {
        	store.put(Names.averageTripFuelEconomy, averageTripFuelEconomy);
        } else {
        	store.remove(Names.averageTripFuelEconomy);
        }
    }
    public Float getAverageTripFuelEconomy() {
        return (Float) store.get(Names.averageTripFuelEconomy);
    }
    public void setCurrentCycleFuelEconomy(Float currentCycleFuelEconomy) {
        if (currentCycleFuelEconomy != null) {
        	store.put(Names.currentCycleFuelEconomy, currentCycleFuelEconomy);
        } else {
        	store.remove(Names.currentCycleFuelEconomy);
        }
    }
    public Float getCurrentCycleFuelEconomy() {
        return (Float) store.get(Names.currentCycleFuelEconomy);
    }
}