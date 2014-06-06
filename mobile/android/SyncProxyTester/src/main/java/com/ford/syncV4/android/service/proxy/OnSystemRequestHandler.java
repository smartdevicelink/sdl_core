package com.ford.syncV4.android.service.proxy;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.adapters.LogAdapter;
import com.ford.syncV4.android.manager.AppPreferencesManager;
import com.ford.syncV4.android.policies.PolicyFilesManager;
import com.ford.syncV4.android.utils.AppUtils;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.proxy.systemrequest.IOnSystemRequestHandler;
import com.ford.syncV4.proxy.systemrequest.ISystemRequestProxy;

import java.util.Arrays;
import java.util.List;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/12/14
 * Time: 2:50 PM
 */
public class OnSystemRequestHandler implements IOnSystemRequestHandler {

    private LogAdapter mLogAdapter;

    public OnSystemRequestHandler(LogAdapter mLogAdapter) {
        this.mLogAdapter = mLogAdapter;
    }

    @Override
    public void onFilesDownloadRequest(final String appId, final ISystemRequestProxy proxy, List<String> urls,
                                       FileType fileType) {

        // Simulate Files downloading request and future processing
        // Then, call appropriate method at provided callback which implement
        // ISystemRequestProxy interface

        new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
            @Override
            public void run() {
                final byte[] data = AppUtils.contentsOfResource(R.raw.audio_short);
                try {
                    proxy.putSystemFile(appId, "system.update", data, FileType.AUDIO_WAVE);
                } catch (SyncException e) {
                    // TODO : Logging to be refactored
                    if (mLogAdapter != null) {
                        mLogAdapter.logMessage("Can't upload system file:" + e.getMessage(),
                                Log.ERROR, true);
                    }
                }
            }
        }, 500);
    }

    @Override
    public void onFileResumeRequest(final String appId, final ISystemRequestProxy proxy, String filename,
                                    final Integer offset, final Integer length, FileType fileType) {

        // Simulate Files download resumption request and future processing
        // Then, call appropriate method at provided callback which implement
        // ISystemRequestProxy interface

        new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
            @Override
            public void run() {
                final byte[] data = Arrays.copyOfRange(
                        AppUtils.contentsOfResource(R.raw.audio_short), offset, offset + length);
                try {
                    proxy.putSystemFile(appId, "system.update", data, offset, FileType.AUDIO_WAVE);
                } catch (SyncException e) {
                    // TODO : Logging to be refactored
                    if (mLogAdapter != null) {
                        mLogAdapter.logMessage("Can't upload system file:" + e.getMessage(),
                                Log.ERROR, true);
                    }
                }
            }
        }, 500);
    }

    @Override
    public void onPolicyTableSnapshotRequest(final String appId, final ISystemRequestProxy proxy,
                                             byte[] data,
                                             final FileType fileType, final RequestType requestType) {
        // TODO : Logging to be refactored
        if (data == null) {
            if (mLogAdapter != null) {
                mLogAdapter.logMessage("Policy Snapshot data is null", Log.ERROR, true);
            }
            return;
        }
        if (mLogAdapter != null) {
            mLogAdapter.logMessage("Policy Table Snapshot download request", Log.DEBUG, true);
        }

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