package com.ford.syncV4.proxy.rpc.enums;

public enum FuelCutoffStatus {
	TERMINATE_FUEL,
	NORMAL_OPERATION,
    FAULT;

    public static FuelCutoffStatus valueForString(String value) {
        return valueOf(value);
    }
}
