package com.ford.syncV4.proxy.rpc.enums;

public enum VehicleDataEventStatus {
	NO_EVENT,
	NO,
	YES,
	NOT_SUPPORTED,
	FAULT;

    public static VehicleDataEventStatus valueForString(String value) {
        return valueOf(value);
    }
}
