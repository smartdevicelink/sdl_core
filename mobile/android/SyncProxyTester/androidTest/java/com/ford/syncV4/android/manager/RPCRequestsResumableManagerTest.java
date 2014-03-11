package com.ford.syncV4.android.manager;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

import junit.framework.TestCase;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/26/14
 * Time: 1:48 PM
 */
public class RPCRequestsResumableManagerTest extends TestCase {

    public void testInitConnectedCollectionEmpty() {
        RPCRequestsResumableManager rpcRequestsResumableManager = new RPCRequestsResumableManager();
        assertEquals(0, rpcRequestsResumableManager.getRequestsConnectedSize());
    }

    public void testInitDisconnectedCollectionEmpty() {
        RPCRequestsResumableManager rpcRequestsResumableManager = new RPCRequestsResumableManager();
        assertEquals(0, rpcRequestsResumableManager.getRequestsDisconnectedSize());
    }

    public void testFillConnectedCollection() {
        RPCRequestsResumableManager rpcRequestsResumableManager = new RPCRequestsResumableManager();
        RPCRequest notification;
        int count = 10;
        for (int i = 0; i < count; i++) {
            notification = new RPCRequest(Names.OnHashChange);
            rpcRequestsResumableManager.addRequestConnected(notification);
        }
        assertEquals(count, rpcRequestsResumableManager.getRequestsConnectedSize());
    }

    public void testFillDisconnectedCollection() {
        RPCRequestsResumableManager rpcRequestsResumableManager = new RPCRequestsResumableManager();
        RPCRequest notification;
        int count = 10;
        for (int i = 0; i < count; i++) {
            notification = new RPCRequest(Names.OnHashChange);
            rpcRequestsResumableManager.addRequestDisconnected(notification);
        }
        assertEquals(count, rpcRequestsResumableManager.getRequestsDisconnectedSize());
    }

    public void testCleanConnectedCollection() {
        RPCRequestsResumableManager rpcRequestsResumableManager = new RPCRequestsResumableManager();
        RPCRequest notification;
        int count = 10;
        for (int i = 0; i < count; i++) {
            notification = new RPCRequest(Names.OnHashChange);
            rpcRequestsResumableManager.addRequestConnected(notification);
        }
        assertEquals(count, rpcRequestsResumableManager.getRequestsConnectedSize());

        rpcRequestsResumableManager.cleanAllRequestsConnected();

        assertEquals(0, rpcRequestsResumableManager.getRequestsConnectedSize());
    }

    public void testCleanDisconnectedCollection() {
        RPCRequestsResumableManager rpcRequestsResumableManager = new RPCRequestsResumableManager();
        RPCRequest notification;
        int count = 10;
        for (int i = 0; i < count; i++) {
            notification = new RPCRequest(Names.OnHashChange);
            rpcRequestsResumableManager.addRequestDisconnected(notification);
        }
        assertEquals(count, rpcRequestsResumableManager.getRequestsDisconnectedSize());

        rpcRequestsResumableManager.cleanAllRequestsDisconnected();

        assertEquals(0, rpcRequestsResumableManager.getRequestsDisconnectedSize());
    }
}