package com.ford.syncV4.proxy.rpc.enums;

public enum AudioType {
    PCM,
    MP3,
    WAV;

    public static AudioType valueForString(String value) {
        return valueOf(value);
    }
}
