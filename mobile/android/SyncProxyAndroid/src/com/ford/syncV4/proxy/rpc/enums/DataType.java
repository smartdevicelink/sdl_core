package com.ford.syncV4.proxy.rpc.enums;

public enum DataType {
    HEX,
    BINARY;

    public static DataType valueForString(String value) {
        return valueOf(value);
    }
}
