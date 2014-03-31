package com.ford.syncV4.android.receivers;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 1/28/14
 * Time: 2:01 PM
 */
public interface ISyncReceiver {

    public void onReceive();

    /**
     * Dispatched when SDK detects Rooted Device
     */
    public void onRootedDevice();
}