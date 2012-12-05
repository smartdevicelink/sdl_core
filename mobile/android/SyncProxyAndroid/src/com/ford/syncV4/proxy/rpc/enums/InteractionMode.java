package com.ford.syncV4.proxy.rpc.enums;

public enum InteractionMode {
    MANUAL_ONLY,
    VR_ONLY,
    BOTH;

    public static InteractionMode valueForString(String value) {
        return valueOf(value);
    }
}
