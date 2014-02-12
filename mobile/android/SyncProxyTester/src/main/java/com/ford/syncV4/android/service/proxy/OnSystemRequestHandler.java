package com.ford.syncV4.android.service.proxy;

import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SafeToast;
import com.ford.syncV4.android.adapters.LogAdapter;
import com.ford.syncV4.android.utils.AppUtils;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.rpc.enums.FileType;
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
    public void onFilesDownloadRequest(final ISystemRequestProxy proxy, List<String> urls,
                                       FileType fileType) {
        // TODO : Logging to be refactored
        if (mLogAdapter != null) {
            mLogAdapter.logMessage("files download request", Log.DEBUG, true);
        }

        // Simulate Files downloading request and future processing
        // Then, call appropriate method at provided callback which implement
        // ISystemRequestProxy interface

        new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
            @Override
            public void run() {
                final byte[] data = AppUtils.contentsOfResource(R.raw.audio_short);
                try {
                    proxy.putSystemFile("system.update", data, FileType.AUDIO_WAVE);
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
    public void onFileResumeRequest(final ISystemRequestProxy proxy, String filename,
                                    final Integer offset, final Integer length, FileType fileType) {
        // TODO : Logging to be refactored
        if (mLogAdapter != null) {
            mLogAdapter.logMessage("files resume request", Log.DEBUG, true);
        }

        // Simulate Files download resumption request and future processing
        // Then, call appropriate method at provided callback which implement
        // ISystemRequestProxy interface

        new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
            @Override
            public void run() {
                final byte[] data = Arrays.copyOfRange(
                        AppUtils.contentsOfResource(R.raw.audio_short), offset, offset + length);
                try {
                    proxy.putSystemFile("system.update", data, offset, FileType.AUDIO_WAVE);
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
    public void onPolicyTableSnapshotRequest(byte[] data) {
        // TODO : Logging to be refactored
        if (mLogAdapter != null) {
            mLogAdapter.logMessage("Policy Table Snapshot download request", Log.DEBUG, true);
        }

        final byte[] fileData = AppUtils.contentsOfResource(R.raw.policy_table_shanpshot);

        String mTMPFilePath = Environment.getExternalStorageDirectory() +
                "/policyTableSnapshot.json";

        boolean result = AppUtils.saveDataToFile(fileData, mTMPFilePath);
        if (result) {
            SafeToast.showToastAnyThread("File '" + mTMPFilePath + "' successfully saved");
        } else {
            SafeToast.showToastAnyThread("File '" + mTMPFilePath + "' could not be save");
        }
    }
}