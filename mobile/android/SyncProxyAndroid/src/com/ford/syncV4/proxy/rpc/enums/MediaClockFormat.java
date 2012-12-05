package com.ford.syncV4.proxy.rpc.enums;

public enum MediaClockFormat {
    CLOCK1,
    CLOCK2,
    CLOCKTEXT1,
    CLOCKTEXT2,
    CLOCKTEXT3;

    public static MediaClockFormat valueForString(String value) {
        return valueOf(value);
    }
}
