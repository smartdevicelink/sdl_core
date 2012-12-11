package com.ford.syncV4.proxy.rpc.enums;

import java.util.EnumSet;

public enum AudioCaptureQuality {
	_8_BIT("8_BIT"),
	_16_BIT("16_BIT");

    String internalName;
    
    private AudioCaptureQuality(String internalName) {
        this.internalName = internalName;
    }
    
    public String toString() {
        return this.internalName;
    }
    
    public static AudioCaptureQuality valueForString(String value) {       	
    	for (AudioCaptureQuality anEnum : EnumSet.allOf(AudioCaptureQuality.class)) {
            if (anEnum.toString().equals(value)) {
                return anEnum;
            }
        }
        return null;
    }
}
