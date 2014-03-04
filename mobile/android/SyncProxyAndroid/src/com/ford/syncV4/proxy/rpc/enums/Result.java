package com.ford.syncV4.proxy.rpc.enums;

/**
 * A list of result codes
 */
public enum Result {
    /**
     * The request succeeded
     */
    SUCCESS,
    /**
     * The RPC (e.g. {@link com.ford.syncV4.proxy.rpc.Slider}) executed successfully and the user
     * selected to save the current position / value
     */
    SAVED,
    /**
     * The data sent is invalid. For example:
     * Invalid Json syntax
     * Parameters out of bounds (number or enum range)
     * Mandatory parameters not provided
     * Parameter provided with wrong type
     * Invalid characters
     * Empty string
     */
    INVALID_DATA,
    /**
     * The request is not supported by Sync
     */
    UNSUPPORTED_REQUEST,
    /**
     * The system could not process the request because the necessary memory couldn't be allocated
     */
    OUT_OF_MEMORY,
    /**
     * There are too many requests pending (means, that the response has not been delivered, yet)
     */
    TOO_MANY_PENDING_REQUESTS,
    /**
     * One of the provided IDs is not valid. For example:
     * this applies to CorrelationID, SubscriptionID, CommandID, MenuID, etc.
     */
    INVALID_ID,
    /**
     * There was a conflict with an registered name (application or menu item) or vr command
     */
    DUPLICATE_NAME,
    /**
     * There are already too many registered applications
     */
    TOO_MANY_APPLICATIONS,
    /**
     * RegisterApplication has been called again, after a RegisterApplication was successful before
     */
    APPLICATION_REGISTERED_ALREADY,
    /**
     * Sync doesn't support the protocol that is requested by the mobile application
     */
    UNSUPPORTED_VERSION,
    /**
     * The requested language is currently not supported. Might be because of a mismatch of the
     * currently active language on Sync and the requested language
     */
    WRONG_LANGUAGE,
    /**
     * An command can not be executed because no application has been registered with
     * RegisterApplication
     */
    APPLICATION_NOT_REGISTERED,
    /**
     * The data may not be changed, because it is currently in use. For example when trying to
     * delete a command set that is currently involved in an interaction
     */
    IN_USE,
    /**
     * The user has turned off access to vehicle data, and it is globally unavailable to mobile
     * applications
     */
    VEHICLE_DATA_NOT_ALLOWED,
    /**
     * The requested vehicle data is not available on this vehicle or is not published
     */
    VEHICLE_DATA_NOT_AVAILABLE,
    /**
     * The requested command was rejected, e.g. because mobile app is in background and cannot
     * perform any HMI commands. Or an HMI command (e.g. {@link com.ford.syncV4.proxy.rpc.Speak})
     * is rejected because a higher priority HMI command
     * (e.g. {@link com.ford.syncV4.proxy.rpc.Alert}) is playing
     */
    REJECTED,
    /**
     * A command was aborted, for example due to user interaction (e.g. user pressed button).
     * Or an HMI command (e.g. {@link com.ford.syncV4.proxy.rpc.Speak}) is aborted because a
     * higher priority HMI command (e.g. {@link com.ford.syncV4.proxy.rpc.Alert}) was requested
     */
    ABORTED,
    /**
     * A command was ignored, because the intended result is already in effect. For example,
     * SetMediaClockTimer was used to pause the media clock although the clock is paused already.
     * NOTE: potentially replaces SUBSCRIBED_ALREADY
     */
    IGNORED,
    /**
     * A button that was requested for subscription is not supported under the current system.
     * NOTE: could become a more generic
     * {@link com.ford.syncV4.proxy.rpc.enums.Result#UNSUPPORTED_RESOURCE} by merging with
     * {@link com.ford.syncV4.proxy.rpc.enums.Result#VEHICLE_DATA_NOT_AVAILABLE}
     */
    UNSUPPORTED_RESOURCE,
    /**
     * A specified file could not be found on Sync
     */
    FILE_NOT_FOUND,
    /**
     * Provided data is valid but something went wrong in the lower layers
     */
    GENERIC_ERROR,
    /**
     * RPC is not authorized in local policy table
     */
    DISALLOWED,
    /**
     * RPC is included in a functional group explicitly blocked by the user
     */
    USER_DISALLOWED,
    /**
     * Overlay reached the maximum timeout and closed
     */
    TIMED_OUT,
    /**
     * User selected to Cancel Route
     */
    CANCEL_ROUTE,
    /**
     * The RPC (e.g. {@link com.ford.syncV4.proxy.rpc.ReadDID}) executed successfully but the data
     * exceeded the platform maximum threshold and thus, only part of the data is available
     */
    TRUNCATED_DATA,
    /**
     * The user interrupted the RPC (e.g. {@link com.ford.syncV4.proxy.rpc.PerformAudioPassThru})
     * and indicated to start over.
     * NOTE, the app must issue the new RPC
     */
    RETRY,
    /**
     * The RPC (e.g. {@link com.ford.syncV4.proxy.rpc.SubscribeVehicleData}) executed successfully
     * but one or more items have a warning or failure
     */
    WARNINGS,
    /**
     * The provided hash ID does not match the hash of the current set of registered data or
     * the core could not resume the previous data
     */
    RESUME_FAILED,
    /**
     * The certificate provided during authentication is invalid
     */
    INVALID_CERT,
    /**
     * The certificate provided during authentication is expired
     */
    EXPIRED_CERT;

    /**
     * Convert {@link java.lang.String} value to {@link com.ford.syncV4.proxy.rpc.enums.Result}
     * object
     *
     * @param value {@link java.lang.String}
     * @return {@link com.ford.syncV4.proxy.rpc.enums.Result} object
     */
    public static Result valueForString(String value) {
        return valueOf(value);
    }
}