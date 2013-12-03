package com.ford.syncV4.proxy.rpc.enums;

import java.util.EnumSet;

public enum HMILevel {
    HMI_FULL("FULL"),
    HMI_LIMITED("LIMITED"),
    HMI_BACKGROUND("BACKGROUND"),
    HMI_NONE("NONE");

    String internalName;
    
    private HMILevel(String internalName) {
        this.internalName = internalName;
    }
    
    public String toString() {
        return this.internalName;
    }
    
    public static HMILevel valueForString(String value) {
        for (HMILevel anEnum : EnumSet.allOf(HMILevel.class)) {
            if (anEnum.toString().equals(value)) {
                return anEnum;
            }
        }
        return null;
    }
}
