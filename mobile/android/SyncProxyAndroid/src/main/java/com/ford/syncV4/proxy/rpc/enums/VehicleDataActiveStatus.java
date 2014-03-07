package com.ford.syncV4.proxy.rpc.enums;

public enum VehicleDataActiveStatus {
	INACTIVE_NOT_CONFIRMED,
	INACTIVE_CONFIRMED,
	ACTIVE_NOT_CONFIRMED,
	ACTIVE_CONFIRMED,
    FAULT;

    public static VehicleDataActiveStatus valueForString(String value) {
        return valueOf(value);
    }
}
