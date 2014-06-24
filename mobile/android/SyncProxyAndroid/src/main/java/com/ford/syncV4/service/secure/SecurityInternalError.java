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

    ERROR_SUCCESS,                    // = 0x00,
    ERROR_INVALID_QUERY_SIZE,         //= 0x01,  // wrong size of query data
    ERROR_INVALID_QUERY_ID,           // = 0x02,  // unknown query id
    ERROR_NOT_SUPPORTED,              //= 0x03,  // SDL does not support encryption
    ERROR_SERVICE_ALREADY_PROTECTED,  //= 0x04,
    ERROR_CREATE_SSLCONTEXT,          //= 0x05,  // could not create new SSLContext
    ERROR_SERVICE_NOT_PROTECTED,      //= 0x06,  // got handshake or encrypted data
                                                // for not protected service ???
    ERROR_DECRYPTION_FAILED,          //= 0x07,
    ERROR_ENCRYPTION_FAILED,          //= 0x08,
    ERROR_SSL_INVALID_DATA,           //= 0xF0,
    ERROR_INTERNAL,                   //= 0xF1,
    ERROR_UNKWOWN_INTERNAL_ERROR,     //= 0xFF
    UNKNOWN

}