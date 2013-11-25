package com.ford.avarsdl.service;

import android.content.ComponentName;
import android.content.ServiceConnection;
import android.os.IBinder;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11/19/13
 * Time: 11:53 AM
 */
public class AppServiceConnectionProxyBase implements ServiceConnection {

    private boolean mIsConnected = false;

    @Override
    public void onServiceConnected(ComponentName name, IBinder service) {
        mIsConnected = true;
    }

    @Override
    public void onServiceDisconnected(ComponentName name) {
        mIsConnected = false;
    }

    public boolean isConnected() {
        return mIsConnected;
    }
}