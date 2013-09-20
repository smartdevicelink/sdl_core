package com.ford.syncV4.proxy.rpc.enums;

public enum ButtonPressMode {
    LONG,
    SHORT;

    public static ButtonPressMode valueForString(String value) {
        return valueOf(value);
    }
}
