package com.ford.syncV4.android.listener;

import android.test.InstrumentationTestCase;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyZeroInteractions;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 1/27/14
 * Time: 12:51 PM
 */
public class ConnectionListenersManagerTest extends InstrumentationTestCase {

    public void testAddListenerAndGetCallback() {
        ConnectionListener connectionListener = mock(ConnectionListener.class);

        ConnectionListenersManager connectionListenersManager = new ConnectionListenersManager();
        ConnectionListenersManager.addConnectionListener(connectionListener);

        connectionListenersManager.dispatch();

        verify(connectionListener).onProxyClosed();
    }

    public void testAddAndRemoveListenerAndNotGetCallback() {
        ConnectionListener connectionListener = mock(ConnectionListener.class);

        ConnectionListenersManager connectionListenersManager = new ConnectionListenersManager();
        ConnectionListenersManager.addConnectionListener(connectionListener);
        ConnectionListenersManager.removeConnectionListener(connectionListener);

        connectionListenersManager.dispatch();

        verifyZeroInteractions(connectionListener);
    }
}