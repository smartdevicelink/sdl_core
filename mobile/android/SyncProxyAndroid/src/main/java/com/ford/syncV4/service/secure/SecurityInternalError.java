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

    ERROR_SUCCESS,                //= 0x0,
    ERROR_INVALID_QUERY_SIZE,     //= 0x1, //low size of message or wrong json_size
    ERROR_INVALID_QUERY_ID,       //= 0x2, //unknown query id
    ERROR_NOT_SUPPORTED,          //= 0x3, //No CryptoManager
    ERROR_SERVICE_ALREADY_PROTECTED, //= 0x4,
    ERROR_CREATE_SLL,             //= 0x5,
    ERROR_CONNECTION_NOT_FOUND,   //= 0x6,
    ERROR_SESSION_NOT_FOUND,      //= 0x7,
    ERROR_SERVICE_NOT_PROTECTED,  //= 0x8, //got handshake for not protected service
    ERROR_SSL_INVALID_DATA,       //= 0xF0,
    ERROR_UNKWOWN_INTERNAL_ERROR, //= 0xFF
    UNKNOWN

}