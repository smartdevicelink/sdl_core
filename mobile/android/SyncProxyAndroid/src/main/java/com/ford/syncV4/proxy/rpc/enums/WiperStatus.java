package com.ford.syncV4.proxy.rpc.enums;

public enum WiperStatus {
	OFF,
	AUTO_OFF,
	OFF_MOVING,
	MAN_INT_OFF,
	MAN_INT_ON,
	MAN_LOW,
	MAN_HIGH,
	MAN_FLICK,
	WASH,
	AUTO_LOW,
	AUTO_HIGH,
	COURTESYWIPE,
	AUTO_ADJUST,
	STALLED,
	NO_DATA_EXISTS;

    public static WiperStatus valueForString(String value) {
        return valueOf(value);
    }
}
