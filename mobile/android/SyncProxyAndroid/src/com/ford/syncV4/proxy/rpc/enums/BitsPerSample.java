package com.ford.syncV4.proxy.rpc.enums;

import java.util.EnumSet;

public enum BitsPerSample {
	_8_BIT("8_BIT"),
	_16_BIT("16_BIT");

    String internalName;
    
    private BitsPerSample(String internalName) {
        this.internalName = internalName;
    }
    
    public String toString() {
        return this.internalName;
    }
    
    public static BitsPerSample valueForString(String value) {       	
    	for (BitsPerSample anEnum : EnumSet.allOf(BitsPerSample.class)) {
            if (anEnum.toString().equals(value)) {
                return anEnum;
            }
        }
        return null;
    }
}
