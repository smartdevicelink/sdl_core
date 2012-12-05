package com.ford.syncV4.proxy.rpc.enums;

public enum FunctionType {
    request,
    response,
    notification;

    public static FunctionType valueForString(String value) {
        return valueOf(value);
    }
}
