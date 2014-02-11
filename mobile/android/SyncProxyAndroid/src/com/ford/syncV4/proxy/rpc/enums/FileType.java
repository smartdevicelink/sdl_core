package com.ford.syncV4.proxy.rpc.enums;

public enum FileType {
    GRAPHIC_BMP,
    GRAPHIC_JPEG,
    GRAPHIC_PNG,
    AUDIO_WAVE,
    AUDIO_MP3,
    AUDIO_AAC,
    BINARY,
    JSON;

    public static FileType valueForString(String value) {
        return valueOf(value);
    }
}
