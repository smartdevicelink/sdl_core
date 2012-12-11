package com.ford.syncV4.proxy.rpc.enums;

public enum VehicleDataResultCode {
	VDRC_SUCCESS,
	VDRC_DISALLOWED,
	VDRC_USER_DISALLOWED,
	VDRC_INVALID_ID,
	VDRC_DATA_NOT_AVAILABLE,
	VDRC_DATA_ALREADY_SUBSCRIBED,
	DATA_NOT_SUBSCRIBED,
	IGNORED;

    public static VehicleDataResultCode valueForString(String value) {
        return valueOf(value);
    }
}
