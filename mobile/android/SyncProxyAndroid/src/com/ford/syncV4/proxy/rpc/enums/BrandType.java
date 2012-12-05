package com.ford.syncV4.proxy.rpc.enums;

public enum BrandType {
    FORD,
    MERCURY,
    LINCOLN;

    public static BrandType valueForString(String value) {
        return valueOf(value);
    }
}
