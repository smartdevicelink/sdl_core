package com.ford.syncV4.proxy.rpc.enums;

public enum TextAlignment {
    LEFT_ALIGNED,
    RIGHT_ALIGNED,
    CENTERED;

    public static TextAlignment valueForString(String value) {
        return valueOf(value);
    }
}
