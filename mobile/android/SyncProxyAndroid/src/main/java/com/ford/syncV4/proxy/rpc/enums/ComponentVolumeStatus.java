package com.ford.syncV4.proxy.rpc.enums;

public enum ComponentVolumeStatus {
	UNKNOWN,
	NORMAL,
	LOW,
	FAULT,
	ALERT,
	NOT_SUPPORTED;

    public static ComponentVolumeStatus valueForString(String value) {
        return valueOf(value);
    }
}
