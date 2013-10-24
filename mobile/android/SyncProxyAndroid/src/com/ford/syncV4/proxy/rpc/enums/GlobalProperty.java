package com.ford.syncV4.proxy.rpc.enums;

public enum GlobalProperty{
    HELPPROMPT,
    TIMEOUTPROMPT,
    VRHELPTITLE,
    KEYBOARDPROPERTIES,
    VRHELPITEMS;

    public static GlobalProperty valueForString(String value) {
        return valueOf(value);
    }
}
