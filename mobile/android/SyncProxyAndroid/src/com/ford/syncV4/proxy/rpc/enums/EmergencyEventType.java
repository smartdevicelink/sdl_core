package com.ford.syncV4.proxy.rpc.enums;

public enum EmergencyEventType {
    NO_EVENT,
    FRONTAL,
    SIDE,
    REAR,
    ROLLOVER,
    NOT_SUPPORTED,
    FAULT;

    public static EmergencyEventType valueForString(String value) {
        return valueOf(value);
    }
}
