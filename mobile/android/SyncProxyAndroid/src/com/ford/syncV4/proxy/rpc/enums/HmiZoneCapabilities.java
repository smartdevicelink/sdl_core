package com.ford.syncV4.proxy.rpc.enums;

public enum HmiZoneCapabilities {
    FRONT,
    BACK;

    public static HmiZoneCapabilities valueForString(String value) {
        return valueOf(value);
    }
}
