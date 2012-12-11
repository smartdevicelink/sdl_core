package com.ford.syncV4.proxy.rpc.enums;

import java.util.EnumSet;

public enum Dimension {
    NO_FIX("NO_FIX"),
    _2D("2D"),
    _3D("3D");
    
    String internalName;

    private Dimension(String internalName) {
    	this.internalName = internalName;
    }
    
    public String toString() {
        return this.internalName;
    }
    
    public static Dimension valueForString(String value) {
    	for (Dimension anEnum : EnumSet.allOf(Dimension.class)) {
            if (anEnum.toString().equals(value)) {
                return anEnum;
            }
        }
        return null;
    }
}
