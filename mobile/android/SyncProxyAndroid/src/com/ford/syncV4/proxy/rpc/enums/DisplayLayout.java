package com.ford.syncV4.proxy.rpc.enums;

public enum DisplayLayout {
    ONSCREEN_PRESETS;

    public static DisplayLayout valueForString(String value) {
        return valueOf(value);
    }
}
