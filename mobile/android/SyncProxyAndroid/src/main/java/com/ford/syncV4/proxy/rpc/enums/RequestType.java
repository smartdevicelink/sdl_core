package com.ford.syncV4.proxy.rpc.enums;

/**
 * Enumeration listing possible asynchronous requests
 */
public enum RequestType {
    HTTP,
    FILE_RESUME,
    AUTH_REQUEST,
    AUTH_CHALLENGE,
    AUTH_ACK,
    PROPRIETARY;

    public static RequestType valueForString(String value) {
        return valueOf(value);
    }
}