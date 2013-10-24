package com.ford.syncV4.proxy.rpc.enums;

public enum DisplayType {
    CID,
    TYPE2,
    TYPE5,
    NGN,
    GEN2_8_DMA,
    GEN2_6_DMA,
    MFD3,
    MFD4,
    MFD5,
    GEN3_8_INCH;

    public static DisplayType valueForString(String value) {
        return valueOf(value);
    }
}
