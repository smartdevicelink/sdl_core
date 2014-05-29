package com.ford.syncV4.android.activity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.telephony.TelephonyManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Spinner;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.android.R;
import com.ford.syncV4.proxy.rpc.DeviceInfo;
import com.ford.syncV4.proxy.rpc.RegisterAppInterface;
import com.ford.syncV4.proxy.rpc.SyncMsgVersion;
import com.ford.syncV4.proxy.rpc.enums.AppHMIType;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.util.DeviceInfoManager;
import com.ford.syncV4.util.logger.Logger;

import java.util.Arrays;
import java.util.Vector;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/25/14
 * Time: 3:19 PM
 */
public class RegisterAppInterfaceDialog extends BaseDialogFragment {

    private static final String LOG_TAG = "RegisterAppInterfaceDialog";

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Context mContext = getActivity();
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        final View layout = inflater.inflate(R.layout.registerappinterface,
                (ViewGroup) getActivity().findViewById(R.id.itemRoot));

        final CheckBox useSyncMsgVersion = (CheckBox) layout
                .findViewById(R.id.registerappinterface_useSyncMsgVersion);
        final EditText syncMsgVersionMajor = (EditText) layout
                .findViewById(R.id.registerappinterface_syncMsgVersionMajor);
        final EditText syncMsgVersionMinor = (EditText) layout
                .findViewById(R.id.registerappinterface_syncMsgVersionMinor);
        final CheckBox useAppName = (CheckBox) layout
                .findViewById(R.id.registerappinterface_useAppName);
        final EditText appName = (EditText) layout.findViewById(R.id.registerappinterface_appName);
        final CheckBox useTTSName = (CheckBox) layout
                .findViewById(R.id.registerappinterface_useTTSName);
        final EditText ttsName = (EditText) layout.findViewById(R.id.registerappinterface_ttsName);
        final CheckBox useNgnAppName = (CheckBox) layout
                .findViewById(R.id.registerappinterface_useNgnAppName);
        final EditText ngnAppName = (EditText) layout
                .findViewById(R.id.registerappinterface_ngnAppName);
        final CheckBox useVRSynonyms = (CheckBox) layout
                .findViewById(R.id.registerappinterface_useVRSynonyms);
        final EditText vrSynonyms = (EditText) layout
                .findViewById(R.id.registerappinterface_vrSynonyms);

        final CheckBox isMediaApp = (CheckBox) layout
                .findViewById(R.id.registerappinterface_isMediaApp);
        final CheckBox useDesiredLang = (CheckBox) layout
                .findViewById(R.id.registerappinterface_useDesiredLang);
        final Spinner desiredLangSpinner = (Spinner) layout
                .findViewById(R.id.registerappinterface_desiredLangSpinner);
        final CheckBox useHMIDesiredLang = (CheckBox) layout
                .findViewById(R.id.registerappinterface_useHMIDesiredLang);
        final Spinner hmiDesiredLangSpinner = (Spinner) layout
                .findViewById(R.id.registerappinterface_hmiDesiredLangSpinner);
        final CheckBox useAppHMITypes = (CheckBox) layout
                .findViewById(R.id.registerappinterface_useAppHMITypes);
        final MultiSpinner<AppHMIType> appHMITypeSpinner = (MultiSpinner) layout
                .findViewById(R.id.registerappinterface_appHMITypeSpinner);
        final CheckBox useAppID = (CheckBox) layout
                .findViewById(R.id.registerappinterface_useAppID);
        final EditText appID =
                (EditText) layout.findViewById(R.id.registerappinterface_appID);

        updateDeviceInfoView(layout);

        final ArrayAdapter<Language> languageAdapter =
                new ArrayAdapter<Language>(mContext,
                        android.R.layout.simple_spinner_item,
                        Language.values());
        languageAdapter.setDropDownViewResource(
                android.R.layout.simple_spinner_dropdown_item);
        // FIXME: use AppHMIType!
        final ArrayAdapter<AppHMIType> appHMITypeAdapter =
                new ArrayAdapter<AppHMIType>(mContext,
                        android.R.layout.simple_spinner_item,
                        AppHMIType.values());
        appHMITypeAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        desiredLangSpinner.setAdapter(languageAdapter);
        hmiDesiredLangSpinner.setAdapter(languageAdapter);
        appHMITypeSpinner.setItems(Arrays.asList(AppHMIType.values()), "All", null);

        return new AlertDialog.Builder(mContext)
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        RegisterAppInterface registerAppInterface = new RegisterAppInterface();
                        registerAppInterface.setCorrelationID(((SyncProxyTester) getActivity()).getNextCorrelationIdForCurrentFragment());

                        if (useSyncMsgVersion.isChecked()) {
                            SyncMsgVersion version = new SyncMsgVersion();
                            String versionStr = null;

                            try {
                                versionStr = syncMsgVersionMinor.getText().toString();
                                version.setMinorVersion(Integer.parseInt(versionStr));
                            } catch (NumberFormatException e) {
                                version.setMinorVersion(2);
                                SafeToast.showToastAnyThread("Couldn't parse minor version " + versionStr);
                            }

                            try {
                                versionStr = syncMsgVersionMajor.getText().toString();
                                version.setMajorVersion(Integer.parseInt(versionStr));
                            } catch (NumberFormatException e) {
                                version.setMajorVersion(2);
                                SafeToast.showToastAnyThread("Couldn't parse major version " + versionStr);
                            }

                            registerAppInterface.setSyncMsgVersion(version);
                        }

                        if (useAppName.isChecked()) {
                            registerAppInterface.setAppName(appName.getText().toString());
                        }
                        if (useTTSName.isChecked()) {
                            registerAppInterface.setTtsName(((SyncProxyTester) getActivity())
                                    .ttsChunksFromString(ttsName.getText().toString()));
                        }
                        if (useNgnAppName.isChecked()) {
                            registerAppInterface.setNgnMediaScreenAppName(ngnAppName.getText().toString());
                        }
                        if (useVRSynonyms.isChecked()) {
                            registerAppInterface.setVrSynonyms(new Vector<String>(Arrays.asList(
                                    vrSynonyms.getText().toString().split(SyncProxyTester.JOIN_STRING))));
                        }
                        registerAppInterface.setIsMediaApplication(isMediaApp.isChecked());
                        if (useDesiredLang.isChecked()) {
                            registerAppInterface.setLanguageDesired(languageAdapter.getItem(
                                    desiredLangSpinner.getSelectedItemPosition()));
                        }
                        if (useHMIDesiredLang.isChecked()) {
                            registerAppInterface.setHmiDisplayLanguageDesired(languageAdapter.getItem(
                                    hmiDesiredLangSpinner.getSelectedItemPosition()));
                        }
                        if (useAppHMITypes.isChecked()) {
                            registerAppInterface.setAppType(new Vector<AppHMIType>(appHMITypeSpinner.getSelectedItems()));
                        }
                        if (useAppID.isChecked()) {
                            registerAppInterface.setAppID(appID.getText().toString());
                        }

                        registerAppInterface.setDeviceInfo(getDeviceInfoFromView(layout));

                        CheckBox createNewSessionView =
                                (CheckBox) layout.findViewById(R.id.registerappinterface_new_session);

                        ((SyncProxyTester) getActivity())
                                .onRegisterAppInterfaceDialogResult(
                                        getArguments().getString(APP_ID_KEY),
                                        registerAppInterface,
                                        createNewSessionView.isChecked());
                    }
                })
                .setNegativeButton("Cancel",
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                dialog.cancel();
                            }
                        })
                .setView(layout)
                .show();
    }

    private void updateDeviceInfoView(View view) {
        DeviceInfo deviceInfo = DeviceInfoManager.getDeviceInfo(
                (TelephonyManager) MainApp.getInstance().getSystemService(Context.TELEPHONY_SERVICE));

        EditText hardwareView = (EditText) view.findViewById(R.id.reg_app_interface_dev_info_hardware);
        EditText firmwareView = (EditText) view.findViewById(R.id.reg_app_interface_dev_info_firmware);
        EditText osView = (EditText) view.findViewById(R.id.reg_app_interface_dev_info_os);
        EditText osVersionView = (EditText) view.findViewById(R.id.reg_app_interface_dev_info_os_ver);
        EditText carrierView = (EditText) view.findViewById(R.id.reg_app_interface_dev_info_carrier);
        EditText maxNumberRFCOMMPortsView = (EditText) view.findViewById(R.id.reg_app_interface_dev_info_max_rf_comm_ports);

        Integer maxRFCOMMPortsValue = deviceInfo.getMaxNumberRFCOMMPorts();
        String maxRFCOMMPortsValueString = (maxRFCOMMPortsValue != null) ? String.valueOf(maxRFCOMMPortsValue) : "0";

        hardwareView.setText(deviceInfo.getHardware());
        firmwareView.setText(deviceInfo.getFirmwareRev());
        osView.setText(deviceInfo.getOS());
        osVersionView.setText(deviceInfo.getOSVersion());
        carrierView.setText(deviceInfo.getCarrier());
        maxNumberRFCOMMPortsView.setText(maxRFCOMMPortsValueString);
    }

    private DeviceInfo getDeviceInfoFromView(View view) {
        DeviceInfo deviceInfo = new DeviceInfo();

        EditText hardwareView = (EditText) view.findViewById(R.id.reg_app_interface_dev_info_hardware);
        EditText firmwareView = (EditText) view.findViewById(R.id.reg_app_interface_dev_info_firmware);
        EditText osView = (EditText) view.findViewById(R.id.reg_app_interface_dev_info_os);
        EditText osVersionView = (EditText) view.findViewById(R.id.reg_app_interface_dev_info_os_ver);
        EditText carrierView = (EditText) view.findViewById(R.id.reg_app_interface_dev_info_carrier);
        EditText maxNumberRFCOMMPortsView = (EditText) view.findViewById(R.id.reg_app_interface_dev_info_max_rf_comm_ports);

        deviceInfo.setHardware(hardwareView.getText().toString());
        deviceInfo.setFirmwareRev(firmwareView.getText().toString());
        deviceInfo.setOS(osView.getText().toString());
        deviceInfo.setOSVersion(osVersionView.getText().toString());
        deviceInfo.setCarrier(carrierView.getText().toString());
        int value = 0;
        try {
            value = Integer.valueOf(maxNumberRFCOMMPortsView.getText().toString());
        } catch (NumberFormatException e) {
            Logger.e(LOG_TAG + " getDeviceInfoFromView " +
                    "maxNumberRFCOMMPortsView error:" + e.getMessage());
        }
        deviceInfo.setMaxNumberRFCOMMPorts(value);

        return deviceInfo;
    }
}