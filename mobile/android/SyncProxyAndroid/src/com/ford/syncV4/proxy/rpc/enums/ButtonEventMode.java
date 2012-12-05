package com.ford.syncV4.proxy.rpc.enums;

public enum ButtonEventMode {
    BUTTONUP,
    BUTTONDOWN;

    public static ButtonEventMode valueForString(String value) {
        return valueOf(value);
    }
}
