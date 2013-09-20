package com.ford.syncV4.proxy.rpc.enums;

public enum VrCapabilities {
    Text;

    public static VrCapabilities valueForString(String value) {
        return valueOf(value);
    }
}
