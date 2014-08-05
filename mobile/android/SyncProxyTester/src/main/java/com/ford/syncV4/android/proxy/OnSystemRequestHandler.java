package com.ford.syncV4.android.proxy;

import android.os.Handler;
import android.os.Looper;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.utils.AppUtils;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.systemrequest.IOnSystemRequestHandler;
import com.ford.syncV4.proxy.systemrequest.IOnSystemRequestHandlerCallback;
import com.ford.syncV4.proxy.systemrequest.ISystemRequestProxy;

import java.util.Arrays;
import java.util.List;

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
 * download files, etc...
 */
public class OnSystemRequestHandler implements IOnSystemRequestHandler {

    private static final String LOG_TAG = OnSystemRequestHandler.class.getSimpleName();

    private IOnSystemRequestHandlerCallback mCallback;

    public OnSystemRequestHandler(IOnSystemRequestHandlerCallback callback) {
        if (callback == null) {
            throw new NullPointerException(LOG_TAG + " Constructor -> " +
                    IOnSystemRequestHandlerCallback.class.getSimpleName() + " implementation is null");
        }
        mCallback = callback;
    }

    @Override
    public void onFilesDownloadRequest(final String appId, final ISystemRequestProxy proxy,
                                       List<String> urls, FileType fileType) {

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
                    mCallback.onError(appId, "Can't upload system file:" + e.getMessage());
                }
            }
        }, 500);
    }

    @Override
    public void onFileResumeRequest(final String appId, final ISystemRequestProxy proxy,
                                    String filename, final Integer offset, final Integer length,
                                    FileType fileType) {

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
                    mCallback.onError(appId, "Can't upload system file:" + e.getMessage());
                }
            }
        }, 500);
    }
}