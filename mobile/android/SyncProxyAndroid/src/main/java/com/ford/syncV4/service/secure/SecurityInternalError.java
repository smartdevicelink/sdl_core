package com.ford.syncV4.service.secure;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/5/14
 * Time: 12:44 PM
 */

/**
 * Response codes for the Secure Service enabling request
 */
public enum SecurityInternalError {
    INVALID_QUERY_SIZE,
    INVALID_QUERY_ID,
    NULL_DATA,
    INVALID_SERVICE_TYPE,
    PROTECTION_NOT_REQUESTED,
    SSL_INVALID_DATA,
    OTHER_INTERNAL_ERROR,
    UNKNOWN
}