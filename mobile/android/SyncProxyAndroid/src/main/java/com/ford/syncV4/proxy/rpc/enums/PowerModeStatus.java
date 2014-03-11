package com.ford.syncV4.proxy.rpc.enums;

public enum PowerModeStatus {
    KEY_OUT,
    KEY_RECENTLY_OUT,
    KEY_APPROVED_0,
    POST_ACCESORY_0,
    ACCESORY_1,
    POST_IGNITION_1,
    IGNITION_ON_2,
    RUNNING_2,
    CRANK_3;

    public static PowerModeStatus valueForString(String value) {
        return valueOf(value);
    }
}
