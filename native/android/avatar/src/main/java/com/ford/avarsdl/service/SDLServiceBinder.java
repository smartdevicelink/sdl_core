package com.ford.avarsdl.service;

import android.os.Binder;

import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11/19/13
 * Time: 11:49 AM
 */
public class SDLServiceBinder extends Binder {

    private SDLService mService;

    public SDLServiceBinder(SDLService service) {
        mService = service;
    }

    public IProxyListenerALM getService() {
        return mService;
    }
}