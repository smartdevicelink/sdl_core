package com.ford.syncV4.proxy.rpc.enums;

public enum GearShiftAdviceStatus {
	NO_INDICATION,
	UPSHIFT_FUEL_ECONOMY,
	UPSHIFT_PERFORMANCE,
	UPSHIFT_WARNING,
	DOWNSHIFT_RECOMMENDATION,
	SHIFT_TO_NEUTRAL;

    public static GearShiftAdviceStatus valueForString(String value) {
        return valueOf(value);
    }
}
