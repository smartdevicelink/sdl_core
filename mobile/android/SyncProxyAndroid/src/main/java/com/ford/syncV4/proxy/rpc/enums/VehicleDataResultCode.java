package com.ford.syncV4.proxy.rpc.enums;

public enum VehicleDataResultCode {
	SUCCESS,
	DISALLOWED,
	USER_DISALLOWED,
	INVALID_ID,
	DATA_NOT_AVAILABLE,
	DATA_ALREADY_SUBSCRIBED,
	DATA_NOT_SUBSCRIBED,
	IGNORED;

    public static VehicleDataResultCode valueForString(String value) {
        return valueOf(value);
    }
}
