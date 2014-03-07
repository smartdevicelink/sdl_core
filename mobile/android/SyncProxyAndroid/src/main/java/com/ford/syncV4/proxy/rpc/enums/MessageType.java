package com.ford.syncV4.proxy.rpc.enums;

public enum MessageType {
    request,
    response,
    notification;

    public static MessageType valueForString(String value) {
        return valueOf(value);
    }
}
