package com.ford.syncV4.android.service;

import android.content.ComponentName;
import android.os.IBinder;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 1/28/14
 * Time: 10:53 AM
 */
public class ProxyServiceConnectionProxy extends AppServiceConnectionProxyBase {

    private IProxyServiceConnection mProxyServiceConnection;

    public ProxyServiceConnectionProxy(IProxyServiceConnection sdlServiceConnection) {
        if (sdlServiceConnection == null) {
            throw new NullPointerException("Can't create ProxyServiceConnectionProxy with NULL parameter");
        }
        mProxyServiceConnection = sdlServiceConnection;
    }

    @Override
    public void onServiceConnected(ComponentName name, IBinder service) {
        super.onServiceConnected(name, service);
        if (service instanceof ProxyServiceBinder) {
            mProxyServiceConnection.onProxyServiceConnected((ProxyServiceBinder) service);
        }
    }

    @Override
    public void onServiceDisconnected(ComponentName name) {
        super.onServiceDisconnected(name);
        if (name != null && name.getClassName() != null && name.getClassName().equals(ProxyService.class.getName())) {
            mProxyServiceConnection.onProxyServiceDisconnected();
        }
    }

    @Override
    public String toString() {
        return getClass().getSimpleName() + '@' + Integer.toHexString(hashCode()) +
                " [holderClass: " + mProxyServiceConnection.getClass().getSimpleName() + "@" +
                Integer.toHexString(mProxyServiceConnection.hashCode()) +
                ", connected: " + isConnected() + "]";
    }
}