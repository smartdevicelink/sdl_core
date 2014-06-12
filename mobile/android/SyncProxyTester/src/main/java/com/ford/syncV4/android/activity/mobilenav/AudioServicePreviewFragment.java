package com.ford.syncV4.android.activity.mobilenav;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.TextView;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SafeToast;
import com.ford.syncV4.android.activity.SyncProxyTester;
import com.ford.syncV4.protocol.enums.ServiceType;

import java.io.OutputStream;

/**
 * Created by Andrew Batutin on 1/23/14
 */
public class AudioServicePreviewFragment extends SyncServiceBaseFragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        return inflater.inflate(R.layout.activity_audio_service_preview, container, false);
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        initiateView(view);
    }

    private void initiateView(View view) {
        mDataStreamingButton = (Button) getView().findViewById(R.id.audio_file_streaming);
        mDataStreamingButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startBaseFileStreaming(R.raw.audio_pcm);
            }
        });
        CheckBox checkBox = (CheckBox) view.findViewById(R.id.audioServiceCheckBox);
        checkBox.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (hasServiceInServicesPool(getAppId(), ServiceType.RPC)) {
                    changeCheckBoxState();
                } else {
                    SafeToast.showToastAnyThread(getString(R.string.rpc_service_not_started));
                }
            }
        });
        Button startCypheredServiceButton = (Button) view.findViewById(R.id.audio_service_secure_checkbox_view);
        startCypheredServiceButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (hasServiceInServicesPool(getAppId(),ServiceType.RPC)) {
                    startSecureAudioService(getAppId());
                } else {
                    SafeToast.showToastAnyThread(getString(R.string.rpc_service_not_started));
                }
            }
        });

        Button startNotCypheredServiceButton = (Button) view.findViewById(R.id.audio_service_not_service_secure_button_view);
        startNotCypheredServiceButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (hasServiceInServicesPool(getAppId(), ServiceType.RPC)) {
                    startNotSecureAudioService(getAppId());
                } else {
                    SafeToast.showToastAnyThread(getString(R.string.rpc_service_not_started));
                }
            }
        });

        mSessionCheckBoxState = new AudioServiceCheckboxState(checkBox, getActivity());
    }


    private void startNotSecureAudioService(String appId) {
        SyncProxyTester syncProxyTester = (SyncProxyTester) getActivity();
        syncProxyTester.startNotSecureAudioService(appId);
    }

    private void startSecureAudioService(String appId) {
        SyncProxyTester syncProxyTester = (SyncProxyTester) getActivity();
        syncProxyTester.startAudioServiceEncryption(appId);
    }

    private void changeCheckBoxState() {
        if (mSessionCheckBoxState.getState().equals(CheckBoxStateValue.OFF)) {
            mSessionCheckBoxState.setStateDisabled();
            SyncProxyTester tester = (SyncProxyTester) getActivity();
            tester.startAudioService(getAppId(), false);
        } else if (mSessionCheckBoxState.getState().equals(CheckBoxStateValue.ON)) {
            mFileStreamingLogic.resetStreaming();
            SyncProxyTester tester = (SyncProxyTester) getActivity();
            tester.stopAudioService(getAppId());
            mSessionCheckBoxState.setStateOff();
            //mDataStreamingButton.setEnabled(false);
        }
    }

    @Override
    public void setStateOff() {
        super.setStateOff();
    }


    public void setAudioServiceStateOn(OutputStream stream, boolean encrypted) {
        mSessionCheckBoxState.setStateOn();
        //mDataStreamingButton.setEnabled(true);

        mFileStreamingLogic.setOutputStream(stream);
        mFileStreamingLogic.createStaticFileReader();
        if (mFileStreamingLogic.isStreamingInProgress()) {
            startBaseFileStreaming(R.raw.audio_pcm);
        }
        notifyAudioServiceState(encrypted);
    }

    private void notifyAudioServiceState(boolean encrypted){
        TextView textView = (TextView) getView().findViewById(R.id.audio_service_status_text_view);
        if (encrypted) {
            textView.setText("Service is cyphered");
        } else {
            textView.setText("Service is Not cyphered");
        }
    }
}