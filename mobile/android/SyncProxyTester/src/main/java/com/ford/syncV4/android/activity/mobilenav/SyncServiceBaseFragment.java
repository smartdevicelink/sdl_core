package com.ford.syncV4.android.activity.mobilenav;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.widget.Button;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.android.listener.ConnectionListener;
import com.ford.syncV4.android.listener.ConnectionListenersManager;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.protocol.enums.ServiceType;

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
    protected CheckBoxState mSessionCheckBoxState;
    protected FileStreamingLogic mFileStreamingLogic;

    private String mAppId = "";

    @Override
    public void onProxyClosed() {
        if (mFileStreamingLogic != null && mFileStreamingLogic.isStreamingInProgress()) {
            mFileStreamingLogic.cancelStreaming();
        }
    }

    @Override
    public void dataStreamingStarted() {
        mDataStreamingButton.setEnabled(false);
        mDataStreamingButton.setText("Data is streaming");
    }

    @Override
    public void dataStreamingStopped() {
        if (mSessionCheckBoxState.getState() == CheckBoxStateValue.ON) {
            mDataStreamingButton.setEnabled(true);
        } else {
            mDataStreamingButton.setEnabled(false);
        }
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

    protected void startBaseFileStreaming(int resourceId) {
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
        mDataStreamingButton.setEnabled(false);
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