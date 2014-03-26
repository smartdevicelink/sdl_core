package com.ford.syncV4.android.activity.mobilenav;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SafeToast;
import com.ford.syncV4.android.activity.SyncProxyTester;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.protocol.enums.ServiceType;

import java.io.OutputStream;

public class MobileNavPreviewFragment extends SyncServiceBaseFragment {

    private static final String LOG_TAG = "MobileNavPreviewFragment";

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        return inflater.inflate(R.layout.activity_mobile_nav_preview, container, true);
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        initiateView(view);
    }

    @Override
    public void setStateOff() {
        super.setStateOff();
    }

    public void setEncryptedMobileNaviServiceStateOff() {
        mEncryptServiceCheckBoxState.setStateOff();
    }

    public void setEncryptedMobileNaviServiceStateOn() {
        mEncryptServiceCheckBoxState.setStateOn();

    }

    public void setMobileNaviStateOn(OutputStream stream) {
        mSessionCheckBoxState.setStateOn();
        //Button button = (Button) getView().findViewById(R.id.videobutton);
        //button.setEnabled(true);
        mDataStreamingButton.setEnabled(true);

        mFileStreamingLogic.setOutputStream(stream);
        mFileStreamingLogic.createStaticFileReader();
        if (mFileStreamingLogic.isStreamingInProgress()) {
            startFileStreaming();
        }
    }

    private void initiateView(View view) {
        mDataStreamingButton = (Button) getView().findViewById(R.id.file_streaming);
        mDataStreamingButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startFileStreaming();
            }
        });
        CheckBox checkBox = (CheckBox) view.findViewById(R.id.mobileNavCheckBox);
        checkBox.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (hasServiceInServicesPool(ServiceType.RPC)) {
                    changeCheckBoxState();
                } else {
                    SafeToast.showToastAnyThread(getString(R.string.rpc_service_not_started));
                }
            }
        });
        CheckBox encryptCheckBoxView = (CheckBox) view.findViewById(R.id.mobile_navi_service_secure_checkbox_view);
        encryptCheckBoxView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (hasServiceInServicesPool(ServiceType.RPC)) {
                    changeEncryptCheckBoxState();
                } else {
                    SafeToast.showToastAnyThread(getString(R.string.rpc_service_not_started));
                }
            }
        });
        mSessionCheckBoxState = new MobileNaviCheckBoxState(checkBox, getActivity());
        mEncryptServiceCheckBoxState = new MobileNaviEncryptCheckBoxState(encryptCheckBoxView,
                getActivity());
    }

    private void changeEncryptCheckBoxState() {
        SyncProxyTester syncProxyTester = (SyncProxyTester) getActivity();
        if (mEncryptServiceCheckBoxState.getState() == CheckBoxStateValue.OFF) {
            mEncryptServiceCheckBoxState.setStateOn();
            syncProxyTester.startMobileNaviServiceEncryption();
        } else if (mEncryptServiceCheckBoxState.getState() == CheckBoxStateValue.ON) {
            mEncryptServiceCheckBoxState.setStateOff();
            syncProxyTester.stopMobileNaviServiceEncryption();
        }
    }

    private void changeCheckBoxState() {
        if (mSessionCheckBoxState.getState().equals(CheckBoxStateValue.OFF)) {
            mSessionCheckBoxState.setStateDisabled();
            SyncProxyTester tester = (SyncProxyTester) getActivity();
            tester.startMobileNaviService();
        } else if (mSessionCheckBoxState.getState().equals(CheckBoxStateValue.ON)) {
            mFileStreamingLogic.resetStreaming();
            SyncProxyTester tester = (SyncProxyTester) getActivity();
            tester.stopMobileNavService();
            mSessionCheckBoxState.setStateOff();
            //Button button = (Button) getView().findViewById(R.id.videobutton);
            //button.setEnabled(false);
        }
    }

    private void startFileStreaming() {
        SharedPreferences prefs = getActivity().getSharedPreferences(Const.PREFS_NAME, 0);
        int videoSource = prefs.getInt(Const.PREFS_KEY_NAVI_VIDEOSOURCE,
                Const.PREFS_DEFAULT_NAVI_VIDEOSOURCE);
        int videoResID;
        switch (videoSource) {
            case Const.KEY_VIDEOSOURCE_MP4:
                videoResID = R.raw.faq_welcome_orientation;
                break;

            case Const.KEY_VIDEOSOURCE_H264:
                videoResID = R.raw.faq_welcome_orientation_rawh264;
                break;

            default:
                Log.e(LOG_TAG, "Unknown video source " + videoSource);
                return;
        }
        startBaseFileStreaming(videoResID);
    }
}