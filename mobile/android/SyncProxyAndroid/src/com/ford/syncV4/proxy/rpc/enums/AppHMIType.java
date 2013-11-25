package com.ford.syncV4.proxy.rpc.enums;

public enum AppHMIType {
    DEFAULT,
    COMMUNICATION,
    MEDIA,
    MESSAGING,
    NAVIGATION,
    INFORMATION,
    SOCIAL,
    BACKGROUND_PROCESS,
    TESTING,
    SYSTEM;

    public static AppHMIType valueForString(String value) {
        return valueOf(value);
    }
}
