package com.ford.syncV4.proxy.rpc.enums;

public enum DisplayType {
    CID("CID"),
    TYPE2("TYPE2"),
    TYPE5("TYPE5"),
    NGN("NGN"),
    GEN2_8_DMA("GEN2_8_DMA"),
    GEN2_6_DMA("GEN2_6_DMA"),
    MFD3("MFD3"),
    MFD4("MFD4"),
    MFD5("MFD5"),
    GEN3_8_INCH("GEN3_8-INCH");

    final String internalName;

    private DisplayType(String internalName) {
        this.internalName = internalName;
    }

    public static DisplayType valueForString(String value) {
        for (DisplayType type : DisplayType.values()) {
            if (type.toString().equals(value)) {
                return type;
            }
        }

        throw new IllegalArgumentException("Unknown value " + value);
    }

    @Override
    public String toString() {
        return internalName;
    }
}
