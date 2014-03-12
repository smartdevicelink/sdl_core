package com.ford.syncV4.proxy.rpc.enums;

/**
 * Created by Andrew Batutin on 9/22/13.
 */
public enum KeyboardEvent {

    KEYPRESS,
    ENTRY_SUBMITTED,
    ENTRY_CANCELLED,
    ENTRY_ABORTED;

    public static KeyboardEvent valueForString(String value) {
        return valueOf(value);
    }

}
