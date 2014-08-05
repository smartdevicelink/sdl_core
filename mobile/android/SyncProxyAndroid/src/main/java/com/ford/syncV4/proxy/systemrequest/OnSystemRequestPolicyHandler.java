package com.ford.syncV4.proxy.systemrequest;

import android.os.Handler;
import android.os.Looper;

import com.ford.syncV4.android.manager.AppPreferencesManager;
import com.ford.syncV4.proxy.policy.PolicyFilesManager;
import com.ford.syncV4.proxy.rpc.OnSystemRequest;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/12/14
 * Time: 2:50 PM
 */

/**
 * This is an implementation of the {@link com.ford.syncV4.proxy.systemrequest.IOnSystemRequestHandler}
 * interface and provides a possibility and main functionality for the handling
 * {@link com.ford.syncV4.proxy.rpc.OnSystemRequest} notifications. In particular, process
 * Policy Table Snapshot (PTS), download files, etc...
 */
public class OnSystemRequestPolicyHandler implements IOnSystemRequestPolicyHandler {

    private static final String LOG_TAG = OnSystemRequestPolicyHandler.class.getSimpleName();

    private IOnSystemRequestHandlerCallback mCallback;

    public OnSystemRequestPolicyHandler(IOnSystemRequestHandlerCallback callback) {
        if (callback == null) {
            throw new NullPointerException(LOG_TAG + " Constructor -> " +
                    IOnSystemRequestHandlerCallback.class.getSimpleName() + " implementation is null");
        }
        mCallback = callback;
    }

    @Override
    public void onPolicyTableSnapshotRequest(final String appId,
                                             final OnSystemRequest onSystemRequest,
                                             final ISystemRequestProxy proxy) {

        if (onSystemRequest == null) {
            mCallback.onError("OnPolicyTableSnapshotRequest -> request is null");
            return;
        }

        if (proxy == null) {
            mCallback.onError("OnPolicyTableSnapshotRequest -> proxy is null");
            return;
        }

        final byte[] data = onSystemRequest.getBulkData();
        final FileType fileType = onSystemRequest.getFileType();
        final RequestType requestType = onSystemRequest.getRequestType();

        if (data == null) {
            mCallback.onError("Policy Snapshot data is null");
            return;
        }
        mCallback.onSuccess("Policy Table Snapshot download request");

        PolicyFilesManager.savePolicyTableSnapshot(data);

        // Simulate Policy Table Snapshot file processing
        // Then, call appropriate method at provided callback which implement
        // ISystemRequestProxy interface

        if (!AppPreferencesManager.getPolicyTableUpdateAutoReplay()) {
            return;
        }

        new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
            @Override
            public void run() {

                PolicyFilesManager.sendPolicyTableUpdate(appId, proxy, fileType, requestType,
                        mLogAdapter);

            }
        }, 500);
    }
}