package com.ford.syncV4.proxy.rpc.enums;

public enum VrCapabilities {
    Text,TEXT;

    public static VrCapabilities valueForString(String value) {
        return valueOf(value);
    }
}
