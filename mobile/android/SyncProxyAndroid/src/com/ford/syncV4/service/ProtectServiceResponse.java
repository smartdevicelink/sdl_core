package com.ford.syncV4.service;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/5/14
 * Time: 12:44 PM
 */

/**
 * Response codes for the Secure Service enabling request
 */
public enum ProtectServiceResponse {
    SUCCESS,
    PENDING,
    SERVICE_ALREADY_PROTECTED,
    SERVICE_NOT_FOUND,
    INTERNAL_ERROR
}