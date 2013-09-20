package com.ford.syncV4.proxy.rpc.enums;

import java.util.EnumSet;

public enum TriggerSource {
    TS_MENU("MENU"),
    TS_VR("VR");

    String internalName;
    
    private TriggerSource(String internalName) {
        this.internalName = internalName;
    }
    
    public String toString() {
        return this.internalName;
    }
    
    public static TriggerSource valueForString(String value) {
        for (TriggerSource anEnum : EnumSet.allOf(TriggerSource.class)) {
            if (anEnum.toString().equals(value)) {
                return anEnum;
            }
        }
        return null;
    }
}
