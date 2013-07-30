package com.ford.syncV4.proxy.rpc.enums;

public enum VehicleDataNotificationStatus {
	NOT_SUPPORTED,
	NORMAL,
	ACTIVE;

    public static VehicleDataNotificationStatus valueForString(String value) {
        return valueOf(value);
    }
}
