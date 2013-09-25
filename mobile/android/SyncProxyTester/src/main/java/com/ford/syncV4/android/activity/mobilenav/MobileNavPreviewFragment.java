package com.ford.syncV4.android.activity.mobilenav;

import android.app.Activity;
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

import java.io.OutputStream;

public class MobileNavPreviewFragment extends Fragment{
    private CheckBoxState mobileNavSessionCheckBoxState;
    private Button dataStreamingButton;
    private FileStreamingLogic fileStreamingLogic;
    private SyncProxyTester context;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.activity_mobile_nav_preview,
                container, true);
        context = (SyncProxyTester) getActivity();
        fileStreamingLogic = new FileStreamingLogic(this);
        return view;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        initiateVideoCheckBox(view);
    }

    private void initiateVideoCheckBox(View view) {
        dataStreamingButton = (Button) getView().findViewById(R.id.file_streaming);
        dataStreamingButton.setOnClickListener(new VideoActionListener());
        CheckBox checkBox = (CheckBox) view.findViewById(R.id.mobileNavCheckBox);
        checkBox.setOnClickListener(new MobileNaviSessionCheckBoxOnClickListener());
        mobileNavSessionCheckBoxState = new MobileNaviCheckBoxState(checkBox, getActivity());
        mobileNavSessionCheckBoxState.setStateOff();
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
            Button button = (Button) getView().findViewById(R.id.videobutton);
            button.setEnabled(false);
            dataStreamingButton.setEnabled(false);
        }
    }

    public void setMobileNaviStateOff(){
        mobileNavSessionCheckBoxState.setStateOff();
        CheckBox box = (CheckBox) getView().findViewById(R.id.mobileNavCheckBox);
        box.setChecked(false);
        Button button = (Button) getView().findViewById(R.id.videobutton);
        button.setEnabled(false);
        dataStreamingButton.setEnabled(false);
    }

    public void setMobileNaviStateOn(OutputStream stream){
        mobileNavSessionCheckBoxState.setStateOn();
        Button button = (Button) getView().findViewById(R.id.videobutton);
        button.setEnabled(true);
        dataStreamingButton.setEnabled(true);
        fileStreamingLogic.setOutputStream(stream);
        fileStreamingLogic.createStaticFileReader();
    }

    public void dataStreamingStarted() {
        dataStreamingButton.setEnabled(false);
        dataStreamingButton.setText("Data is streaming");
    }

    public void dataStreamingStopped() {
        if (mobileNavSessionCheckBoxState.getState() == CheckBoxStateValue.ON){
            dataStreamingButton.setEnabled(true);
        }else{
            dataStreamingButton.setEnabled(false);
        }
        dataStreamingButton.setText("Start File Streaming");
    }

    private void startFileStreaming() {
        fileStreamingLogic.startFileStreaming();
    }

    private class MobileNaviSessionCheckBoxOnClickListener implements View.OnClickListener{

        @Override
        public void onClick(View v) {
            onMobileNaviCheckBoxAction(v);
        }
    }

    private class VideoActionListener implements View.OnClickListener{

        @Override
        public void onClick(View v) {
            startFileStreaming();
        }
    }

}
