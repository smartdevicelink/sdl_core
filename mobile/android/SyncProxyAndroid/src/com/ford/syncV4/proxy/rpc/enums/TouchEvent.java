package com.ford.syncV4.proxy.rpc.enums;

public enum TouchEvent {
	TOUCHSTART,
	TOUCHMOVE,
	TOUCHEND,
	DOUBLETOUCH;

    public static TouchEvent valueForString(String value) {
        return valueOf(value);
    }
}