package com.ford.syncV4.proxy.rpc.enums;

import java.util.EnumSet;

public enum SystemContext {
    SYSCTXT_MAIN("MAIN"),
    SYSCTXT_VRSESSION("VRSESSION"),
    SYSCTXT_MENU("MENU"),
    SYSCTXT_HMI_OBSCURED("HMI_OBSCURED"),
    SYSCTXT_ALERT("ALERT");

    String internalName;
    
    private SystemContext(String internalName) {
        this.internalName = internalName;
    }
    
    public String toString() {
        return this.internalName;
    }
    
    public static SystemContext valueForString(String value) {
        for (SystemContext anEnum : EnumSet.allOf(SystemContext.class)) {
            if (anEnum.toString().equals(value)) {
                return anEnum;
            }
        }
        return null;
    }
}
