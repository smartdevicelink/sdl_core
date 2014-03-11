package com.ford.syncV4.proxy.rpc.enums;

public enum CarModeStatus {
	NORMAL,
	FACTORY,
	TRANSPORT,
	CRASH;

    public static CarModeStatus valueForString(String value) {
        return valueOf(value);
    }
}
