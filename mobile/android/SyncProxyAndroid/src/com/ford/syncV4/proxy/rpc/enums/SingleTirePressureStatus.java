package com.ford.syncV4.proxy.rpc.enums;

public enum SingleTirePressureStatus {
	UNKNOWN,
	NORMAL,
	LOW,
	FAULT,
	NOT_SUPPORTED;

    public static SingleTirePressureStatus valueForString(String value) {
        return valueOf(value);
    }
}
