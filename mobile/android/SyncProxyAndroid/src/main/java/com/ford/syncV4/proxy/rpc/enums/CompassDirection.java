package com.ford.syncV4.proxy.rpc.enums;

public enum CompassDirection {
	NORTH,
	NORTHWEST,
	WEST,
	SOUTHWEST,
	SOUTH,
	SOUTHEAST,
	EAST,
	NORTHEAST;

    public static CompassDirection valueForString(String value) {
        return valueOf(value);
    }
}
