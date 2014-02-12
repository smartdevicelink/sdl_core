package com.ford.syncV4.android.service.proxy;

import android.os.Handler;
import android.os.Looper;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.systemrequest.IOnSystemRequestHandler;
import com.ford.syncV4.proxy.systemrequest.ISystemRequestProxy;

import java.util.List;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/12/14
 * Time: 2:50 PM
 */
public class OnSystemRequestHandler implements IOnSystemRequestHandler {

    @Override
    public void onFilesDownloadRequest(ISystemRequestProxy proxy, List<String> urls,
                                       FileType fileType) {
        //createDebugMessageForAdapter("files download request");

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
                    createErrorMessageForAdapter("Can't upload system file", e);
                }
            }
        }, 500);
    }

    @Override
    public void onFileResumeRequest(ISystemRequestProxy proxy, String filename, Integer offset,
                                    Integer length, FileType fileType) {

    }

    @Override
    public void onPolicyTableSnapshotRequest(byte[] data) {

    }
}