package com.ford.syncV4.proxy.rpc.enums;

public enum TBTState {
    ROUTE_UPDATE_REQUEST,
    ROUTE_ACCEPTED,
    ROUTE_REFUSED,
    ROUTE_CANCELLED,
    ETA_REQUEST,
    NEXT_TURN_REQUEST,
    ROUTE_STATUS_REQUEST,
    ROUTE_SUMMARY_REQUEST,
    TRIP_STATUS_REQUEST,
    ROUTE_UPDATE_REQUEST_TIMEOUT;

    public static TBTState valueForString(String value) {
        return valueOf(value);
    }
}
