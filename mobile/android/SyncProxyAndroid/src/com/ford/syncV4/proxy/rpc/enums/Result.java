package com.ford.syncV4.proxy.rpc.enums;

public enum Result {
    SUCCESS,
    INVALID_DATA,
    UNSUPPORTED_REQUEST,
    OUT_OF_MEMORY,
    TOO_MANY_PENDING_REQUESTS,
    INVALID_ID,
    DUPLICATE_NAME,
    TOO_MANY_APPLICATIONS,
    APPLICATION_REGISTERED_ALREADY,
    UNSUPPORTED_VERSION,
    WRONG_LANGUAGE,
    APPLICATION_NOT_REGISTERED,
    IN_USE,
    SUBSCRIBED_ALREADY,
    VEHICLE_DATA_NOT_ALLOWED,
    VEHICLE_DATA_NOT_AVAILABLE,
    REJECTED,
    ABORTED,
    IGNORED,
    RETRY,
    UNSUPPORTED_BUTTON,
    FILE_NOT_FOUND,
    GENERIC_ERROR,
    DISALLOWED,
    USER_DISALLOWED,
    TIMED_OUT,
    CANCEL_ROUTE,
    CLOSED,
    ENCRYPTED,
    WARNINGS;

    public static Result valueForString(String value) {
        return valueOf(value);
    }
}
