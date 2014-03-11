package com.ford.syncV4.proxy.rpc.enums;

public enum DeviceLevelStatus {
	ZERO_LEVEL_BARS,
	ONE_LEVEL_BARS,
	TWO_LEVEL_BARS,
	THREE_LEVEL_BARS,
	FOUR_LEVEL_BARS,
	NOT_PROVIDED;

    public static DeviceLevelStatus valueForString(String value) {
        return valueOf(value);
    }
}
