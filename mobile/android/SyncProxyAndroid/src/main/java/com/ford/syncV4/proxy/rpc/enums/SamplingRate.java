package com.ford.syncV4.proxy.rpc.enums;

import java.util.EnumSet;

public enum SamplingRate {
	_8KHZ("8KHZ"),
	_16KHZ("16KHZ"),
	_22KHZ("22KHZ"),
	_44KHZ("44KHZ");

    String internalName;
    
    private SamplingRate(String internalName) {
        this.internalName = internalName;
    }
    
    public String toString() {
        return this.internalName;
    }
    
    public static SamplingRate valueForString(String value) {       	
    	for (SamplingRate anEnum : EnumSet.allOf(SamplingRate.class)) {
            if (anEnum.toString().equals(value)) {
                return anEnum;
            }
        }
        return null;
    }
}
