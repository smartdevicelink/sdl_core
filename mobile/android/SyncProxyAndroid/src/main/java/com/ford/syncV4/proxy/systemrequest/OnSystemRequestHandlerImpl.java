package com.ford.syncV4.proxy.systemrequest;

import android.os.Handler;
import android.os.Looper;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.net.IDataDownloader;
import com.ford.syncV4.net.PolicyDataDownloaderImpl;
import com.ford.syncV4.net.parser.RequestAbstractDataParser;
import com.ford.syncV4.net.parser.RequestJSONDataParser;
import com.ford.syncV4.proxy.policy.PolicyFilesManager;
import com.ford.syncV4.proxy.rpc.OnSystemRequest;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.service.IDataServiceProvider;
import com.ford.syncV4.service.PolicyDataServiceProviderImpl;
import com.ford.syncV4.test.TestConfig;

import java.util.List;
import java.util.Vector;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/12/14
 * Time: 2:50 PM
 */

/**
 * This is an implementation of the {@link IOnSystemRequestHandler}
 * interface and provides a possibility and main functionality for the handling
 * {@link com.ford.syncV4.proxy.rpc.OnSystemRequest} notifications. In particular, process
 * of the files downloading, policy, etc...
 */
public class OnSystemRequestHandlerImpl implements IOnSystemRequestHandler {

    private static final String LOG_TAG = OnSystemRequestHandlerImpl.class.getSimpleName();

    private IOnSystemRequestHandlerCallback mCallback;

    /**
     * This is a field which stores all the configured data for the SDK testing
     * purposes and shot NOT be included in the release.
     */
    private TestConfig mTestConfig;

    public OnSystemRequestHandlerImpl(IOnSystemRequestHandlerCallback callback) {
        if (callback == null) {
            throw new NullPointerException(LOG_TAG + " Constructor -> " +
                    IOnSystemRequestHandlerCallback.class.getSimpleName() + " implementation is null");
        }
        mCallback = callback;
    }

    public void setTestConfig(TestConfig value) {
        mTestConfig = value;
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
                // TODO : Implement fake data here but in actual it is necessary to provide data from SDL
                final byte[] data = new byte[100];
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
                //final byte[] data = Arrays.copyOfRange(
                //        AppUtils.contentsOfResource(R.raw.audio_short), offset, offset + length);
                // TODO : Implement fake data here but in actual it is necessary to provide data from SDL
                final byte[] data = new byte[100];
                try {
                    proxy.putSystemFile(appId, "system.update", data, offset, FileType.AUDIO_WAVE);
                } catch (SyncException e) {
                    mCallback.onError(appId, "Can't upload system file:" + e.getMessage());
                }
            }
        }, 500);
    }

    @Override
    public void onPolicyTableSnapshotRequest(final String appId,
                                             final OnSystemRequest onSystemRequest,
                                             final ISystemRequestProxy proxy) {

        if (onSystemRequest == null) {
            mCallback.onError(appId, "OnPolicyTableSnapshotRequest -> request is null");
            return;
        }

        if (proxy == null) {
            mCallback.onError(appId, "OnPolicyTableSnapshotRequest -> proxy is null");
            return;
        }

        final byte[] data = onSystemRequest.getBulkData();
        final FileType fileType = onSystemRequest.getFileType();
        final RequestType requestType = onSystemRequest.getRequestType();

        if (data == null) {
            mCallback.onError(appId, " Policy Snapshot data is null");
            return;
        }

        // TODO : Temporary solution
        Vector<String> test_urls = new Vector<String>(1);
        test_urls.add("http://policies.telematicsqa.ford.com/api/policies");
        onSystemRequest.setUrl(test_urls);

        Vector<String> urls = onSystemRequest.getUrl();
        if (urls == null) {
            mCallback.onError(appId, " Policy Snapshot url is null");
            return;
        }
        if (urls.isEmpty()) {
            mCallback.onError(appId, " Policy Snapshot url is empty");
            return;
        }

        String url = onSystemRequest.getUrl().get(0);

        mCallback.onSuccess(appId, "Policy Table Snapshot download request ...");

        RequestAbstractDataParser dataParser = new RequestJSONDataParser();
        IDataDownloader downloader = new PolicyDataDownloaderImpl();
        IDataServiceProvider serviceProvider = new PolicyDataServiceProviderImpl(dataParser);
        byte[] responseData = serviceProvider.getPolicyTableUpdateData(downloader, url,
                onSystemRequest.getBulkData());

        if (responseData == null) {
            mCallback.onSuccess(appId, "Policy Table Snapshot download complete, data null");
            return;
        } else {
            mCallback.onSuccess(appId, "Policy Table Snapshot download complete, " +
                    "data size:" + responseData.length);
        }

        // Test Config:
        if (mTestConfig != null) {
            if (mTestConfig.isDoOverridePolicyTableUpdateData()) {
                responseData = mTestConfig.getPolicyTableUpdateData();
            }
        }

        PolicyFilesManager.sendPolicyTableUpdate(appId, proxy, fileType, requestType, responseData,
                mCallback);
    }
}