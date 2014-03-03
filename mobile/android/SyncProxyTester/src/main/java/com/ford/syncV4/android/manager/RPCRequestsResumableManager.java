package com.ford.syncV4.android.manager;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/26/14
 * Time: 12:25 PM
 */

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.rpc.PutFile;

import java.util.Vector;

/**
 * This class provide functionality to manage RPC Requests which are involved in application
 * resumption procedure
 */
public class RPCRequestsResumableManager {

    public interface RPCRequestsResumableManagerCallback {
        public void onSendRequest(RPCRequest request);
    }

    // This Vector keep all RPC requests since the last successful application start in case of
    // connection exists
    private final Vector<RPCRequest> rpcRequestsResumableConnected = new Vector<RPCRequest>();
    // This Vector keep all RPC requests since the last successful application start in case of
    // connection does not exists
    private final Vector<RPCRequest> rpcRequestsResumableDisconnected = new Vector<RPCRequest>();

    // This Vector keep all PutFiles since the last successful application start
    private final Vector<PutFile> putFiles = new Vector<PutFile>();

    private RPCRequestsResumableManagerCallback callback;

    /**
     * Constructor
     */
    public RPCRequestsResumableManager() {

    }

    /**
     * Set callback handler to inform about outgoing requests
     *
     * @param callback {@link com.ford.syncV4.android.manager.RPCRequestsResumableManager.RPCRequestsResumableManagerCallback}
     */
    public void setCallback(RPCRequestsResumableManagerCallback callback) {
        this.callback = callback;
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
     * Add {@link com.ford.syncV4.proxy.rpc.PutFile} request to the collection
     *
     * @param putFile {@link com.ford.syncV4.proxy.rpc.PutFile} request
     */
    public void addPutFile(PutFile putFile) {
        putFiles.add(putFile);
    }

    /**
     * Send all RPC requests which were add when there were connection to SDL.
     *
     * @throws SyncException
     */
    public void sendAllRequestsConnected() throws SyncException {
        for (RPCRequest request : rpcRequestsResumableConnected) {
            if (callback != null) {
                callback.onSendRequest(request);
            }
        }
    }

    /**
     * Send all RPC requests which were add when there were no connection to SDL.
     *
     * @throws SyncException
     */
    public void sendAllRequestsDisconnected() throws SyncException {
        for (RPCRequest request : rpcRequestsResumableDisconnected) {
            if (callback != null) {
                callback.onSendRequest(request);
            }
        }
    }

    /**
     * Send all RPC requests which were add since last success connection
     *
     * @throws SyncException
     */
    public void sendAllPutFiles() throws SyncException {
        for (PutFile putFile : putFiles) {
            if (callback != null) {
                callback.onSendRequest(putFile);
            }
        }
    }

    /**
     * Clean all RPC requests which were add when there were connection to SDL.
     */
    public void cleanAllRequestsDisconnected() {
        rpcRequestsResumableDisconnected.clear();
    }

    /**
     * Clean all {@link com.ford.syncV4.proxy.rpc.PutFile}'s requests collection
     */
    public void cleanAllPutFiles() {
        putFiles.clear();
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

    /**
     * @return a size of the {@link com.ford.syncV4.proxy.rpc.PutFile}'s requests collection
     */
    public int getPutFilesSize() {
        return putFiles.size();
    }
}