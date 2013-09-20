package com.ford.syncV4.proxy.rpc.enums;

public enum AudioStreamingState {
    AUDIBLE,
    ATTENUATED,
    NOT_AUDIBLE;

    public static AudioStreamingState valueForString(String value) {
        return valueOf(value);
    }
}
