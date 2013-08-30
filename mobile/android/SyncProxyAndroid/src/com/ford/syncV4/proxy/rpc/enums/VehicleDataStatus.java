package com.ford.syncV4.proxy.rpc.enums;

public enum VehicleDataStatus {
	NO_DATA_EXISTS,
    OFF,
    ON;

    public static VehicleDataStatus valueForString(String value) {
        return valueOf(value);
    }
}
