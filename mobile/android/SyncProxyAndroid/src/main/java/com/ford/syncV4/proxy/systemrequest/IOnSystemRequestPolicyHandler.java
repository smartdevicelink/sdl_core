package com.ford.syncV4.proxy.systemrequest;

import com.ford.syncV4.proxy.rpc.OnSystemRequest;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 05.08.14
 * Time: 13:03
 */

/**
 * Interface for handling special OnSystemRequest notifications: Policy.
 */
public interface IOnSystemRequestPolicyHandler {

    /**
     * Requests to get Policy Table Snapshot file as raw data and process it. When done, result
     * should be upload with
     *
     * @param proxy           The proxy used to upload the given file
     * @param onSystemRequest {@link com.ford.syncV4.proxy.rpc.OnSystemRequest} notification
     * @param appId           Application identifier
     */
    public void onPolicyTableSnapshotRequest(String appId, OnSystemRequest onSystemRequest,
                                             ISystemRequestProxy proxy);
}