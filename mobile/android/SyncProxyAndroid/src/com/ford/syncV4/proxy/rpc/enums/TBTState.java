package com.ford.syncV4.proxy.rpc.enums;

public enum TBTState {
	ROUTE_CANCELLED,
	DESTINATION_REACHED,
	ROUTE_UPDATE_MANUAL,
	OFF_ROUTE_UPDATE,
	OFF_ROUTE_DECLINED,
	ROUTE_ACCEPTED, 
	ROUTE_REFUSED;

    public static TBTState valueForString(String value) {
        return valueOf(value);
    }
}
