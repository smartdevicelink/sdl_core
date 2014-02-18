package com.ford.syncV4.android.activity.mobilenav;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SafeToast;
import com.ford.syncV4.android.activity.SyncProxyTester;
import com.ford.syncV4.protocol.enums.ServiceType;

import java.io.OutputStream;

/**
 * Created by Andrew Batutin on 1/23/14.
 */
public class AudioServicePreviewFragment extends SyncServiceBaseFragment {


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        return inflater.inflate(R.layout.activity_audio_service_preview, container, true);
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
                if (hasServiceInServicesPool(ServiceType.RPC)) {
                    onMobileNaviCheckBoxAction(view);
                } else {
                    SafeToast.showToastAnyThread(getString(R.string.rpc_service_not_started));
                }
            }
        });
        mSessionCheckBoxState = new AudioServiceCheckboxState(checkBox, getActivity());
        mSessionCheckBoxState.setStateOff();
    }

    public void onMobileNaviCheckBoxAction(View v) {
        changeMobileNaviCheckBoxState();
    }

    private void changeMobileNaviCheckBoxState() {
        if (mSessionCheckBoxState.getState().equals(CheckBoxStateValue.OFF)) {
            mSessionCheckBoxState.setStateDisabled();
            SyncProxyTester tester = (SyncProxyTester) getActivity();
            tester.startAudioService();
        } else if (mSessionCheckBoxState.getState().equals(CheckBoxStateValue.ON)) {
            mFileStreamingLogic.resetStreaming();
            SyncProxyTester tester = (SyncProxyTester) getActivity();
            tester.stopAudioService();
            mSessionCheckBoxState.setStateOff();
            mDataStreamingButton.setEnabled(false);
        }
    }

    @Override
    public void setStateOff() {
        super.setStateOff();
        CheckBox box = (CheckBox) getView().findViewById(R.id.audioServiceCheckBox);
        box.setChecked(false);
    }

    public void setAudioServiceStateOn(OutputStream stream) {
        mSessionCheckBoxState.setStateOn();
        mDataStreamingButton.setEnabled(true);

        mFileStreamingLogic.setOutputStream(stream);
        mFileStreamingLogic.createStaticFileReader();
        if (mFileStreamingLogic.isStreamingInProgress()) {
            startBaseFileStreaming(R.raw.audio_pcm);
        }
    }
}