package com.ford.syncV4.session;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 03.06.14
 * Time: 15:35
 */
public enum EndServiceInitiator {

    /**
     * Mobile SDK is the one who instantiate End Service
     */
    SDK,

    /**
     * SDL is the one who instantiate End Service
     */
    SDL
}