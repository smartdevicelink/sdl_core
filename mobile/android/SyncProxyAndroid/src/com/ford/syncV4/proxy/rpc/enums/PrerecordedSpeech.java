package com.ford.syncV4.proxy.rpc.enums;

public enum PrerecordedSpeech {
    HELP_JINGLE,
    INITIAL_JINGLE,
    LISTEN_JINGLE,
    POSITIVE_JINGLE,
    NEGATIVE_JINGLE;

    public static PrerecordedSpeech valueForString(String value) {
        return valueOf(value);
    }
}
