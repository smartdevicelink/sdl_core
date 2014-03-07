package com.ford.syncV4.proxy.rpc.enums;

public enum IgnitionStableStatus {
	IGNITION_SWITCH_NOT_STABLE,
	IGNITION_SWITCH_STABLE,
	MISSING_FROM_TRANSMITTER;

    public static IgnitionStableStatus valueForString(String value) {
        return valueOf(value);
    }
}
