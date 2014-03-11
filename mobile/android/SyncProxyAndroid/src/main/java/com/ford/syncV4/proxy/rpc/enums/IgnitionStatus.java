package com.ford.syncV4.proxy.rpc.enums;

public enum IgnitionStatus {
	UNKNOWN,
	OFF,
	ACCESSORY,
	RUN,
	START,
	INVALID;

    public static IgnitionStatus valueForString(String value) {
        return valueOf(value);
    }
}
