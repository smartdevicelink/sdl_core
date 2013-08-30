package com.ford.syncV4.proxy.rpc.enums;

public enum MaintenanceModeStatus {
	NORMAL,
	NEAR,
	ACTIVE,
	FEATURE_NOT_PRESENT;

    public static MaintenanceModeStatus valueForString(String value) {
        return valueOf(value);
    }
}
