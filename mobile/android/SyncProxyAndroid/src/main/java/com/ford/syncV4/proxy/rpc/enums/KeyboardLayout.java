package com.ford.syncV4.proxy.rpc.enums;

public enum KeyboardLayout {
    QWERTY,
    QWERTZ,
    AZERTY;

    public static KeyboardLayout valueForString(String value) {
        return valueOf(value);
    }
}
