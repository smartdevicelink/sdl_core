package com.ford.syncV4.android.manager;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/26/14
 * Time: 12:25 PM
 */

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.SyncProxyALM;

import java.util.Vector;

/**
 * This class provide functionality to manage RPC Requests which are involved in application
 * resumption procedure
 */
public class RPCRequestsResumableManager {

    // This Vector keep all RPC requests since the last successful application start in case of
    // connection exists
    private final Vector<RPCRequest> rpcRequestsResumableConnected = new Vector<RPCRequest>();
    // This Vector keep all RPC requests since the last successful application start in case of
    // connection does not exists
    private final Vector<RPCRequest> rpcRequestsResumableDisconnected = new Vector<RPCRequest>();

    /**
     * Constructor
     */
    public RPCRequestsResumableManager() {

    }

    /**
     * Add RPC request to the collection in case of connection exists
     *
     * @param request {@link com.ford.syncV4.proxy.RPCRequest} object
     */
    public void addRequestConnected(RPCRequest request) {
        rpcRequestsResumableConnected.add(request);
    }

    /**
     * Add RPC request to the collection in case of connection does not exists
     *
     * @param request {@link com.ford.syncV4.proxy.RPCRequest} object
     */
    public void addRequestDisconnected(RPCRequest request) {
        rpcRequestsResumableDisconnected.add(request);
    }

    /**
     * Send all RPC requests which were add when there were connection to SDL.
     *
     * @param syncProxy {@link com.ford.syncV4.proxy.SyncProxyALM} proxy object
     * @throws SyncException
     */
    public void sendAllRequestsConnected(SyncProxyALM syncProxy) throws SyncException {
        for (RPCRequest request : rpcRequestsResumableConnected) {
            syncProxy.sendRPCRequest(request);
        }
    }

    /**
     * Send all RPC requests which were add when there were no connection to SDL.
     *
     * @param syncProxy {@link com.ford.syncV4.proxy.SyncProxyALM} proxy object
     * @throws SyncException
     */
    public void sendAllRequestsDisconnected(SyncProxyALM syncProxy) throws SyncException {
        for (RPCRequest request : rpcRequestsResumableDisconnected) {
            syncProxy.sendRPCRequest(request);
        }
    }

    /**
     * Clean all RPC requests which were add when there were connection to SDL.
     */
    public void cleanAllRequestsDisconnected() {
        rpcRequestsResumableDisconnected.clear();
    }

    /**
     * Clean all RPC requests which were add when there were no connection to SDL.
     */
    public void cleanAllRequestsConnected() {
        rpcRequestsResumableConnected.clear();
    }

    /**
     * Return a size of the RPC requests collection
     *
     * @return a size of the collection
     */
    public int getRequestsConnectedSize() {
        return rpcRequestsResumableConnected.size();
    }

    /**
     * Return a size of the RPC requests collection
     *
     * @return a size of the collection
     */
    public int getRequestsDisconnectedSize() {
        return rpcRequestsResumableDisconnected.size();
    }
}