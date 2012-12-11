package com.ford.syncV4.proxy.rpc.enums;

public enum PRNDL {
	PARK,
	REVERSE,
	NEUTRAL,
	LOWGEAR,
	FORWARD_DRIVE_2,
	FORWARD_DRIVE_3,
	FORWARD_DRIVE_4,
	FORWARD_DRIVE_5,
	FORWARD_DRIVE_6,
	SECOND_GEAR_HOLD,
	THIRD_GEAR_HOLD,
	FOURTH_GEAR_HOLD,
	FIFTH_GEAR_HOLD,
	SIXTH_GEAR_HOLD;

    public static PRNDL valueForString(String value) {
        return valueOf(value);
    }
}
