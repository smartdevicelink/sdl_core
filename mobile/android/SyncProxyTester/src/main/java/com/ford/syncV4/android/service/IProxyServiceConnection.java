package com.ford.syncV4.android.service;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11/19/13
 * Time: 11:56 AM
 */
public interface IProxyServiceConnection {
    void onProxyServiceConnected(ProxyServiceBinder service);
    void onProxyServiceDisconnected();
}