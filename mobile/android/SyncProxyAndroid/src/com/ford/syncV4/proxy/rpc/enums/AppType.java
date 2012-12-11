package com.ford.syncV4.proxy.rpc.enums;

public enum AppType {
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

    public static AppType valueForString(String value) {
        return valueOf(value);
    }
}
