package com.ford.syncV4.android.listener;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 1/27/14
 * Time: 11:11 AM
 */
public class ConnectionListenersManager {

    private static final List<WeakReference<ConnectionListener>> mConnectivityListeners =
            new ArrayList<WeakReference<ConnectionListener>>();

    public ConnectionListenersManager() {

    }

    /**
     * Dispatch connection events described at ConnectionListener to the subscribed objects
     */
    public void dispatch() {
        for (WeakReference<ConnectionListener> reference : mConnectivityListeners) {
            ConnectionListener callback = reference.get();
            if (callback != null) {
                callback.onProxyClosed();
            }
        }
    }

    /**
     * Add listener reference
     * @param listener implementation of ConnectionListener interface
     */
    public static synchronized void addConnectionListener(ConnectionListener listener) {
        mConnectivityListeners.add(new WeakReference<ConnectionListener>(listener));
    }

    /**
     * Remove listener reference
     * @param listener implementation of ConnectionListener interface
     */
    public static synchronized void removeConnectionListener(ConnectionListener listener) {
        for (WeakReference<ConnectionListener> reference : mConnectivityListeners) {
            ConnectionListener connectionListener = reference.get();
            if (listener.equals(connectionListener)) {
                mConnectivityListeners.remove(reference);
                return;
            }
        }
    }
}