package com.ford.syncV4.android.activity.mobilenav;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SyncProxyTester;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.android.listener.ConnectionListener;
import com.ford.syncV4.android.listener.ConnectionListenersManager;

import java.io.OutputStream;

public class MobileNavPreviewFragment extends Fragment implements ServicePreviewFragmentInterface,
        ConnectionListener {

    private static final String TAG = MobileNavPreviewFragment.class.getSimpleName();
    private CheckBoxState mobileNavSessionCheckBoxState;
    private Button dataStreamingButton;
    private FileStreamingLogic fileStreamingLogic;
    private SyncProxyTester context;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        addListeners();
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.activity_mobile_nav_preview, container, true);
        context = (SyncProxyTester) getActivity();
        fileStreamingLogic = new FileStreamingLogic(this);
        return view;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        initiateVideoCheckBox(view);
    }

    @Override
    public void dataStreamingStarted() {
        dataStreamingButton.setEnabled(false);
        dataStreamingButton.setText("Data is streaming");
    }

    @Override
    public void dataStreamingStopped() {
        if (mobileNavSessionCheckBoxState.getState() == CheckBoxStateValue.ON) {
            dataStreamingButton.setEnabled(true);
        } else {
            dataStreamingButton.setEnabled(false);
        }
        dataStreamingButton.setText("Start File Streaming");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        removeListeners();
    }

    @Override
    public void onProxyClosed() {
        if (fileStreamingLogic != null && fileStreamingLogic.isStreamingInProgress()) {
            fileStreamingLogic.cancelStreaming();
        }
    }

    public void onMobileNaviCheckBoxAction(View v) {
        changeMobileNaviCheckBoxState();
    }

    public void setMobileNaviStateOff() {
        mobileNavSessionCheckBoxState.setStateOff();
        CheckBox box = (CheckBox) getView().findViewById(R.id.mobileNavCheckBox);
        box.setChecked(false);
        Button button = (Button) getView().findViewById(R.id.videobutton);
        button.setEnabled(false);
        dataStreamingButton.setEnabled(false);
    }

    public void setMobileNaviStateOn(OutputStream stream) {
        mobileNavSessionCheckBoxState.setStateOn();
        Button button = (Button) getView().findViewById(R.id.videobutton);
        button.setEnabled(true);
        dataStreamingButton.setEnabled(true);

        fileStreamingLogic.setOutputStream(stream);
        fileStreamingLogic.createStaticFileReader();
        Log.d(TAG, "MobileNaviStateOn, inProgress:" + fileStreamingLogic.isStreamingInProgress());
        if (fileStreamingLogic.isStreamingInProgress()) {
            startFileStreaming();
        }
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

    private void initiateVideoCheckBox(View view) {
        dataStreamingButton = (Button) getView().findViewById(R.id.file_streaming);
        dataStreamingButton.setOnClickListener(new VideoActionListener());
        CheckBox checkBox = (CheckBox) view.findViewById(R.id.mobileNavCheckBox);
        checkBox.setOnClickListener(new MobileNaviSessionCheckBoxOnClickListener());
        mobileNavSessionCheckBoxState = new MobileNaviCheckBoxState(checkBox, getActivity());
        mobileNavSessionCheckBoxState.setStateOff();
    }

    private void changeMobileNaviCheckBoxState() {
        if (mobileNavSessionCheckBoxState.getState().equals(CheckBoxStateValue.OFF)) {
            mobileNavSessionCheckBoxState.setStateDisabled();
            SyncProxyTester tester = (SyncProxyTester) getActivity();
            tester.startMobileNaviService();
        } else if (mobileNavSessionCheckBoxState.getState().equals(CheckBoxStateValue.ON)) {
            fileStreamingLogic.resetStreaming();
            SyncProxyTester tester = (SyncProxyTester) getActivity();
            tester.stopMobileNavService();
            mobileNavSessionCheckBoxState.setStateOff();
            Button button = (Button) getView().findViewById(R.id.videobutton);
            button.setEnabled(false);
        }
    }

    private void startFileStreaming() {
        SharedPreferences prefs = context.getSharedPreferences(Const.PREFS_NAME, 0);
        int videoSource = prefs.getInt(Const.PREFS_KEY_NAVI_VIDEOSOURCE,
                Const.PREFS_DEFAULT_NAVI_VIDEOSOURCE);

        int videoResID = 0;
        switch (videoSource) {
            case Const.KEY_VIDEOSOURCE_MP4:
                videoResID = R.raw.faq_welcome_orientation;
                break;

            case Const.KEY_VIDEOSOURCE_H264:
                videoResID = R.raw.faq_welcome_orientation_rawh264;
                break;

            default:
                Log.e(TAG, "Unknown video source " + videoSource);
                break;
        }

        fileStreamingLogic.setFileResID(videoResID);
        fileStreamingLogic.startFileStreaming();
    }

    private class MobileNaviSessionCheckBoxOnClickListener implements View.OnClickListener {

        @Override
        public void onClick(View v) {
            onMobileNaviCheckBoxAction(v);
        }
    }

    private class VideoActionListener implements View.OnClickListener {

        @Override
        public void onClick(View v) {
            startFileStreaming();
        }
    }
}