package com.ford.syncV4.android.service;

import android.os.Binder;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 1/28/14
 * Time: 10:45 AM
 */
public class ProxyServiceBinder extends Binder {

    private ProxyService mProxyService;

    public ProxyServiceBinder(ProxyService proxyService) {
        mProxyService = proxyService;
    }

    public ProxyService getService() {
        return mProxyService;
    }
}