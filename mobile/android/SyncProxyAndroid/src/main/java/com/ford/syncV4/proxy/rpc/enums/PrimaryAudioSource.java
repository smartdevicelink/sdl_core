package com.ford.syncV4.proxy.rpc.enums;

public enum PrimaryAudioSource {
    NO_SOURCE_SELECTED,
    USB,
    USB2,
    BLUETOOTH_STEREO_BTST,
    LINE_IN,
    IPOD,
    MOBILE_APP;

    public static PrimaryAudioSource valueForString(String value) {
        return valueOf(value);
    }
}
