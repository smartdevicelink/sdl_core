package com.ford.syncV4.proxy.rpc.enums;

public enum UpdateMode {
    COUNTUP,
    COUNTDOWN,
    PAUSE,
    RESUME,
    CLEAR;

    public static UpdateMode valueForString(String value) {
        return valueOf(value);
    }
}
