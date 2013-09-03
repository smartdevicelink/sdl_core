package com.ford.syncV4.android.activity.mobilenav;

import android.app.Activity;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SyncProxyTester;

public class MobileNavPreviewFragment extends Fragment implements VideoDataListener {

    private static final String logTag = "MobileNavPreviewFragment";
    private CheckBoxState videoCheckBoxState;
    private CheckBoxState mobileNavSessionCheckBoxState;
    private MockVideoDataSource videoDataSource;

    public MockVideoDataSource getVideoDataSource() {
        return videoDataSource;
    }

    public VideoCheckBoxState getVideoCheckBoxState() {
        return (VideoCheckBoxState) videoCheckBoxState;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        Log.i(logTag, "MobileNavPreviewFragment created");
        View view = inflater.inflate(R.layout.activity_mobile_nav_preview,
                container, true);
        return view;
    }

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        Log.i(logTag, "activity attached to  MobileNavPreviewFragment");
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        initiateVideoCheckBox(view);
    }

    @Override
    public void onPause() {
        super.onPause();
        Log.i(logTag, "activity paused");
        if (videoDataSource != null) {
            videoCheckBoxState.setStateDisabled();
            videoDataSource.stop();
        }
    }

    private void initiateVideoCheckBox(View view) {
        CheckBox box = (CheckBox) view.findViewById(R.id.videoStreamingCheckBox);
        videoCheckBoxState = new VideoCheckBoxState(box, getActivity());
        videoCheckBoxState.setStateOff();
        mobileNavSessionCheckBoxState = new MobileNaviCheckBoxState((CheckBox) view.findViewById(R.id.mobileNavCheckBox), getActivity());
        mobileNavSessionCheckBoxState.setStateOff();
    }

    public void onVideoStreamingCheckBoxAction(View checkBox) {
        changeVideoCheckBoxState((CheckBox) checkBox);
    }

    private void changeVideoCheckBoxState(CheckBox checkBox) {
        if (videoCheckBoxState.getState().equals(CheckBoxStateValue.OFF)) {
            videoDataSource = new MockVideoDataSource(this);
            videoCheckBoxState.setStateDisabled();
            videoDataSource.start();
        } else if (videoCheckBoxState.getState().equals(CheckBoxStateValue.ON)) {
            videoCheckBoxState.setStateDisabled();
            videoDataSource.stop();
        }
    }

    public void onMobileNaviCheckBoxAction(View v) {
        changeMobileNaviCheckBoxState();
    }

    private void changeMobileNaviCheckBoxState() {
        if (mobileNavSessionCheckBoxState.getState().equals(CheckBoxStateValue.OFF)) {
            mobileNavSessionCheckBoxState.setStateDisabled();
            SyncProxyTester tester = (SyncProxyTester) getActivity();
            tester.startMobileNaviSession();
        } else if (mobileNavSessionCheckBoxState.getState().equals(CheckBoxStateValue.ON)) {
            SyncProxyTester tester = (SyncProxyTester) getActivity();
            tester.stopMobileNavSession();
            mobileNavSessionCheckBoxState.setStateOff();
        }
    }

    public void setMobileNaviStateOff(){
        mobileNavSessionCheckBoxState.setStateOff();
        CheckBox box = (CheckBox) getView().findViewById(R.id.mobileNavCheckBox);
        box.setChecked(false);
    }

    @Override
    public void onStreamingStart() {
        videoCheckBoxState.setStateOn();
        Log.i(logTag, "video streaming started");

    }

    @Override
    public void videoFrameReady(byte[] videoFrame) {
        Log.d(logTag, "video frame received" + videoFrame.toString());
    }

    @Override
    public void onStreamStop() {
        videoCheckBoxState.setStateOff();
        Log.i(logTag, "video streaming stopped");

    }
}
