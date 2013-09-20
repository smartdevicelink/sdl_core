package com.ford.syncV4.proxy.rpc.enums;

public enum SpeechCapabilities {
    TEXT,
    SAPI_PHONEMES,
    LHPLUS_PHONEMES,
    PRE_RECORDED,
    SILENCE;

    public static SpeechCapabilities valueForString(String value) {
        return valueOf(value);
    }
}
