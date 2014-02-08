package com.ford.syncV4.proxy.rpc.enums;

public enum PRNDL {
    PARK,
    REVERSE,
    NEUTRAL,
    DRIVE,
    SPORT,
    LOWGEAR,
    FIRST,
    SECOND,
    THIRD,
    FOURTH,
    FIFTH,
    SIXTH,
    SEVENTH,
    EIGHTH,
    UNKNOWN,
    FAULT;

    public static PRNDL valueForString(String value) {
        return valueOf(value);
    }
}
