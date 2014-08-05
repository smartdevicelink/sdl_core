package com.ford.syncV4.proxy.policy;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/26/14
 * Time: 9:59 AM
 */

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.proxy.systemrequest.IOnSystemRequestHandlerCallback;
import com.ford.syncV4.proxy.systemrequest.ISystemRequestProxy;

/**
 * This class provide possibility to save Policy Table Snapshot file (which comes from SDL)
 * and to send back Policy Table Update file
 */
public class PolicyFilesManager {

    /**
     * Send Policy Table Update data to SDL
     *
     * @param proxy    {@link com.ford.syncV4.proxy.systemrequest.ISystemRequestProxy} implementation
     * @param fileType type of the file
     * @param callback implementation of the
     *                 {@link com.ford.syncV4.proxy.systemrequest.IOnSystemRequestHandlerCallback}
     *                 interface
     */
    public static void sendPolicyTableUpdate(String appId, ISystemRequestProxy proxy,
                                             FileType fileType, RequestType requestType,
                                             byte[] data,
                                             IOnSystemRequestHandlerCallback callback) {
        try {
            String policyTableUpdateFileName = "PolicyTableUpdate";
            proxy.putPolicyTableUpdateFile(appId, policyTableUpdateFileName, data, fileType,
                    requestType);
            if (callback != null) {
                //SafeToast.showToastAnyThread("Policy Update sent");
                callback.onSuccess(appId, "Policy Update sent");
            }
        } catch (SyncException e) {
            if (callback != null) {
                callback.onError(appId, "Can't upload policy table update mPolicyUpdateFile:" +
                        e.getMessage());
            }
        }
    }
}