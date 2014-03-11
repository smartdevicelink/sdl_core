package com.ford.syncV4.proxy.rpc.enums;

import java.util.EnumSet;

public enum SoftButtonType {
	SBT_TEXT("TEXT"),
	SBT_IMAGE("IMAGE"),
	SBT_BOTH("BOTH");

    String internalName;
    
    private SoftButtonType(String internalName) {
        this.internalName = internalName;
    }
    
    public String toString() {
        return this.internalName;
    }
    
    public static SoftButtonType valueForString(String value) {       	
    	for (SoftButtonType anEnum : EnumSet.allOf(SoftButtonType.class)) {
            if (anEnum.toString().equals(value)) {
                return anEnum;
            }
        }
        return null;
    }
}
