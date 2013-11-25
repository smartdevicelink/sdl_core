package com.ford.avarsdl.service;

import android.content.ComponentName;
import android.os.IBinder;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11/19/13
 * Time: 11:55 AM
 */
public class SDLServiceConnectionProxy extends AppServiceConnectionProxyBase {

    private ISDLServiceConnection mSDLServiceConnection;

    public SDLServiceConnectionProxy(ISDLServiceConnection sdlServiceConnection) {
        if (sdlServiceConnection == null) {
            throw new NullPointerException("Can't create SDLServiceConnectionProxy with NULL parameter");
        }
        mSDLServiceConnection = sdlServiceConnection;
    }

    @Override
    public void onServiceConnected(ComponentName name, IBinder service) {
        super.onServiceConnected(name, service);
        if (service instanceof SDLServiceBinder) {
            mSDLServiceConnection.onSDLServiceConnected((SDLServiceBinder) service);
        }
    }

    @Override
    public void onServiceDisconnected(ComponentName name) {
        super.onServiceDisconnected(name);
        if (name != null && name.getClassName() != null && name.getClassName().equals(SDLService.class.getName())) {
            mSDLServiceConnection.onSDLServiceDisconnected();
        }
    }

    @Override
    public String toString() {
        return getClass().getSimpleName() + '@' + Integer.toHexString(hashCode()) +
                " [holderClass: " + mSDLServiceConnection.getClass().getSimpleName() + "@" +
                Integer.toHexString(mSDLServiceConnection.hashCode()) +
                ", connected: " + isConnected() + "]";
    }
}