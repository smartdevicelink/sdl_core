package com.ford.syncV4.android.activity.mobilenav;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.widget.Button;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.android.activity.SyncProxyTester;
import com.ford.syncV4.android.listener.ConnectionListener;
import com.ford.syncV4.android.listener.ConnectionListenersManager;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.util.logger.Logger;

import java.io.OutputStream;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/6/14
 * Time: 3:18 PM
 */
public class SyncServiceBaseFragment extends Fragment implements ServicePreviewFragmentInterface,
                                                                 ConnectionListener {

    @SuppressWarnings("unused")
    private static final String TAG = SyncServiceBaseFragment.class.getSimpleName();

    protected Button mDataStreamingButton;
    protected Button mStopDataStreamingButton;
    protected CheckBoxState mSessionCheckBoxState;
    protected FileStreamingLogic mFileStreamingLogic;

    private String mAppId = "";
    private ServiceType mServiceType = null;

    @Override
    public void onProxyClosed() {
        Logger.d(TAG + " proxy closed");
        if (mFileStreamingLogic != null && mFileStreamingLogic.isStreamingInProgress()) {
            mFileStreamingLogic.cancelStreaming();
        }
    }

    @Override
    public void dataStreamingStarted() {
        mDataStreamingButton.setText("Data is streaming");
    }

    @Override
    public void dataStreamingStopped() {
        mDataStreamingButton.setText("Start File Streaming");
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mFileStreamingLogic = new FileStreamingLogic(this);
        addListeners();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        removeListeners();
    }

    public String getAppId() {
        return mAppId;
    }

    public void setAppId(String value) {
        mAppId = value;
    }

    public ServiceType getServiceType() {
        if (mServiceType == null) {
            throw new NullPointerException(((Object) this).getClass().getSimpleName() +
                    " get ServiceType is NULL," +
                    " probably it has not been initialized in the super class");
        }
        return mServiceType;
    }

    public void setServiceType(ServiceType value) {
        mServiceType = value;
    }

    protected void startBaseFileStreaming(int resourceId) {
        if (mFileStreamingLogic.getOutputStream() == null) {
            OutputStream outputStream = ((SyncProxyTester) getActivity())
                    .getOutputStreamForService(getAppId(), getServiceType());
            mFileStreamingLogic.setOutputStream(outputStream);
        }
        mFileStreamingLogic.setFileResID(resourceId);
        mFileStreamingLogic.startFileStreaming();
    }

    protected boolean hasServiceInServicesPool(String appId, ServiceType serviceType) {
        if (serviceType == null) {
            return false;
        }
        ProxyService proxyService = MainApp.getInstance().getBoundProxyService();
        return proxyService != null && proxyService.hasServiceInServicesPool(appId, serviceType);
    }

    protected void setStateOff() {
        mFileStreamingLogic.resetStreaming();
        mSessionCheckBoxState.setStateOff();
    }

    /**
     * Add all necessary listeners
     */
    private void addListeners() {
        ConnectionListenersManager.addConnectionListener(this);
    }

    /**
     * Remove all subscribed listeners
     */
    private void removeListeners() {
        ConnectionListenersManager.removeConnectionListener(this);
    }
}