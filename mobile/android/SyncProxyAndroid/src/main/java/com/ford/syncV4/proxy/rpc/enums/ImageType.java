package com.ford.syncV4.proxy.rpc.enums;

public enum ImageType {
    STATIC,
    DYNAMIC;

    public static ImageType valueForString(String value) {
        return valueOf(value);
    }
}
