package com.ford.syncV4.android.activity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.os.Build;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RadioGroup;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.ToggleButton;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.android.manager.AppIdManager;
import com.ford.syncV4.android.manager.AppPreferencesManager;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.proxy.SyncProxyBase;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.transport.TransportType;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 12/24/13
 * Time: 1:32 PM
 */

/**
 * Shows a dialog where the user can select connection features (media flag, app name, language,
 * HMI language, transport settings, etc ...).
 * Starts the proxy after selecting.
 */
public class AppSetUpDialog extends DialogFragment {

    private static final String LOG_TAG = "AppSetUpDialog";

    public static AppSetUpDialog newInstance() {
        AppSetUpDialog appSetupDialog = new AppSetUpDialog();
        return appSetupDialog;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        Context context = getActivity();
        LayoutInflater inflater = (LayoutInflater) context.getSystemService(
                getActivity().LAYOUT_INFLATER_SERVICE);
        final View view = inflater.inflate(R.layout.selectprotocol,
                (ViewGroup) getActivity().findViewById(R.id.selectprotocol_Root));

        ArrayAdapter<Language> langAdapter = new ArrayAdapter<Language>(getActivity(),
                android.R.layout.simple_spinner_item, Language.values());
        langAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        final CheckBox policyFileUpdateAutoReplayView =
                (CheckBox) view.findViewById(R.id.policy_file_update_auto_replay_view);
        policyFileUpdateAutoReplayView.setChecked(AppPreferencesManager.getPolicyTableUpdateAutoReplay());
        policyFileUpdateAutoReplayView.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                AppPreferencesManager.setPolicyTableUpdateAutoReplay(isChecked);
            }
        });
        final CheckBox isHearBeat = (CheckBox) view.findViewById(R.id.heartbeat);
        final CheckBox mediaCheckBox = (CheckBox) view.findViewById(R.id.selectprotocol_checkMedia);
        final CheckBox naviCheckBox = (CheckBox) view.findViewById(
                R.id.selectprotocol_checkMobileNavi);
        final RadioGroup videoSourceGroup = (RadioGroup) view.findViewById(
                R.id.selectprotocol_radioGroupVideoSource);
        final EditText appNameEditText = (EditText) view.findViewById(R.id.selectprotocol_appName);
        final Spinner langSpinner = (Spinner) view.findViewById(R.id.selectprotocol_lang);
        final Spinner hmiLangSpinner = (Spinner) view.findViewById(R.id.selectprotocol_hmiLang);
        final RadioGroup transportGroup = (RadioGroup) view.findViewById(
                R.id.selectprotocol_radioGroupTransport);
        final EditText ipAddressEditText = (EditText) view.findViewById(R.id.selectprotocol_ipAddr);
        final EditText tcpPortEditText = (EditText) view.findViewById(R.id.selectprotocol_tcpPort);
        final LinearLayout nsdUseLayout = (LinearLayout) view.findViewById(R.id.nsd_use_layout);
        final LinearLayout ipAddressLayout = (LinearLayout) view.findViewById(R.id.ip_address_layout);
        final LinearLayout portLayout = (LinearLayout) view.findViewById(R.id.port_layout);
        final ToggleButton mNSDUseToggle = (ToggleButton) view.findViewById(R.id.nsd_toggle_btn);

        final boolean mIsNSDSupported = Build.VERSION.SDK_INT >= Const.JELLYBEAN_API_LEVEL;

        final CheckBox autoSetAppIconCheckBox = (CheckBox) view.findViewById(
                R.id.selectprotocol_checkAutoSetAppIcon);

        ipAddressLayout.setVisibility(View.GONE);
        portLayout.setVisibility(View.GONE);
        nsdUseLayout.setVisibility(View.GONE);

        final CheckBox customAppIdView = (CheckBox) view.findViewById(R.id.selectprotocol_appIdCustom);
        customAppIdView.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                processCustomAppIdCheck(view, isChecked);
            }
        });
        customAppIdView.setChecked(AppPreferencesManager.getIsCustomAppId());

        final EditText customAppIdEditView = (EditText) view.findViewById(R.id.selectprotocol_appId);
        customAppIdEditView.setText(AppPreferencesManager.getCustomAppId());
        /*customAppIdEditView.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
                //Log.d(LOG_TAG, "App Id before changed to: " + customAppIdEditView.getText().toString().trim());
            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                //Log.d(LOG_TAG, "App Id on changed to: " + customAppIdEditView.getText().toString().trim());
            }

            @Override
            public void afterTextChanged(Editable s) {
                Log.d(LOG_TAG, "App Id after changed to: " +
                        customAppIdEditView.getText().toString().trim());
                AppPreferencesManager.setCustomAppId(customAppIdEditView.getText().toString().trim());
            }
        });*/

        transportGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {

                TransportType transportType = TransportType.USB;
                switch (checkedId) {
                    case R.id.selectprotocol_radioWiFi:
                        transportType = TransportType.TCP;
                        break;
                    case R.id.selectprotocol_radioBT:
                        transportType = TransportType.BLUETOOTH;
                        break;
                }
                AppPreferencesManager.setTransportType(transportType);

                if (!AppPreferencesManager.getIsCustomAppId()) {
                    processCustomAppIdCheck(view, customAppIdView.isChecked());
                }

                boolean transportOptionsEnabled = checkedId == R.id.selectprotocol_radioWiFi;
                ipAddressLayout.setVisibility(transportOptionsEnabled ? View.VISIBLE : View.GONE);
                portLayout.setVisibility(transportOptionsEnabled ? View.VISIBLE : View.GONE);
                nsdUseLayout.setVisibility(transportOptionsEnabled ? View.VISIBLE : View.GONE);
                if (!mIsNSDSupported) {
                    showNSDUnsupportedView(view);
                }
            }
        });

        langSpinner.setAdapter(langAdapter);
        hmiLangSpinner.setAdapter(langAdapter);

        // display current configs
        final SharedPreferences prefs = getActivity().getSharedPreferences(Const.PREFS_NAME, 0);
        boolean isMedia = prefs.getBoolean(Const.PREFS_KEY_ISMEDIAAPP,
                Const.PREFS_DEFAULT_ISMEDIAAPP);
        boolean isNavi = prefs.getBoolean(Const.PREFS_KEY_ISNAVIAPP,
                Const.PREFS_DEFAULT_ISNAVIAPP);
        int videoSource = prefs.getInt(Const.PREFS_KEY_NAVI_VIDEOSOURCE,
                Const.PREFS_DEFAULT_NAVI_VIDEOSOURCE);
        String appName = prefs.getString(Const.PREFS_KEY_APPNAME,
                Const.PREFS_DEFAULT_APPNAME);
        Language lang = Language.valueOf(prefs.getString(Const.PREFS_KEY_LANG,
                Const.PREFS_DEFAULT_LANG));
        Language hmiLang = Language.valueOf(prefs.getString(
                Const.PREFS_KEY_HMILANG, Const.PREFS_DEFAULT_HMILANG));
        String ipAddress = prefs.getString(
                Const.Transport.PREFS_KEY_TRANSPORT_IP,
                Const.Transport.PREFS_DEFAULT_TRANSPORT_IP);
        int tcpPort = prefs.getInt(Const.Transport.PREFS_KEY_TRANSPORT_PORT,
                Const.Transport.PREFS_DEFAULT_TRANSPORT_PORT);

        boolean autoSetAppIcon = prefs.getBoolean(
                Const.PREFS_KEY_AUTOSETAPPICON,
                Const.PREFS_DEFAULT_AUTOSETAPPICON);

        mediaCheckBox.setChecked(isMedia);
        naviCheckBox.setChecked(isNavi);
        appNameEditText.setText(appName);
        langSpinner.setSelection(langAdapter.getPosition(lang));
        hmiLangSpinner.setSelection(langAdapter.getPosition(hmiLang));
        ipAddressEditText.setText(ipAddress);
        tcpPortEditText.setText(String.valueOf(tcpPort));
        mNSDUseToggle.setChecked(prefs.getBoolean(Const.Transport.PREFS_KEY_IS_NSD, false));
        autoSetAppIconCheckBox.setChecked(autoSetAppIcon);

        int groupCheck = R.id.selectprotocol_radioUSB;
        TransportType transportType = AppPreferencesManager.getTransportType();
        switch (transportType) {
            case TCP:
                groupCheck = R.id.selectprotocol_radioWiFi;
                break;
            case BLUETOOTH:
                groupCheck = R.id.selectprotocol_radioBT;
                break;
        }
        transportGroup.check(groupCheck);

        videoSourceGroup.check(videoSource == Const.KEY_VIDEOSOURCE_MP4 ?
                R.id.selectprotocol_radioSourceMP4 :
                R.id.selectprotocol_radioSourceH264);

        return new AlertDialog.Builder(context)
                .setTitle(getString(R.string.app_setup_dialog_title))
                .setCancelable(false)
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                        if (AppPreferencesManager.getIsCustomAppId()) {
                            AppPreferencesManager.setCustomAppId(
                                    customAppIdEditView.getText().toString().trim());
                        }

                        boolean isMedia = mediaCheckBox.isChecked();
                        boolean isNavi = naviCheckBox.isChecked();
                        int videoSource = (videoSourceGroup.getCheckedRadioButtonId() ==
                                R.id.selectprotocol_radioSourceMP4 ? Const.KEY_VIDEOSOURCE_MP4 :
                                Const.KEY_VIDEOSOURCE_H264);
                        String appName = appNameEditText.getText().toString();
                        String lang = ((Language) langSpinner.getSelectedItem()).name();
                        String hmiLang = ((Language) hmiLangSpinner.getSelectedItem()).name();
                        String ipAddress = ipAddressEditText.getText().toString();
                        int tcpPort = Integer.parseInt(tcpPortEditText.getText().toString());
                        boolean autoSetAppIcon = autoSetAppIconCheckBox.isChecked();
                        boolean mNSDPrefValue = mIsNSDSupported && mNSDUseToggle.isChecked();
                        // save the configs
                        boolean success = prefs
                                .edit()
                                .putBoolean(Const.PREFS_KEY_ISMEDIAAPP, isMedia)
                                .putBoolean(Const.PREFS_KEY_ISNAVIAPP, isNavi)
                                .putBoolean(Const.Transport.PREFS_KEY_IS_NSD, mNSDPrefValue)
                                .putInt(Const.PREFS_KEY_NAVI_VIDEOSOURCE, videoSource)
                                .putString(Const.PREFS_KEY_APPNAME, appName)
                                .putString(Const.PREFS_KEY_LANG, lang)
                                .putString(Const.PREFS_KEY_HMILANG, hmiLang)
                                .putString(Const.Transport.PREFS_KEY_TRANSPORT_IP, ipAddress)
                                .putInt(Const.Transport.PREFS_KEY_TRANSPORT_PORT, tcpPort)
                                .putBoolean(Const.PREFS_KEY_AUTOSETAPPICON, autoSetAppIcon)
                                .commit();
                        if (!success) {
                            Log.w(LOG_TAG, "Can't save selected protocol properties");
                        }

                        setupHeartbeat(isHearBeat);
                        ((SyncProxyTester) getActivity()).onSetUpDialogResult();
                    }
                }).setView(view).show();
    }

    private void setupHeartbeat(CheckBox isHearBeat) {
        if (isHearBeat.isChecked()) {
            SyncProxyBase.setHeartBeatInterval(ProxyService.HEARTBEAT_INTERVAL);
        } else {
            SyncProxyBase.setHeartBeatInterval(ProxyService.HEARTBEAT_INTERVAL_MAX);
        }
    }

    private void showNSDUnsupportedView(View view) {
        TextView mNSDUnsupportedView = (TextView) view.findViewById(R.id.nsd_unsupported_api_label_view);
        TextView mNSDLabelView = (TextView) view.findViewById(R.id.nsd_label_view);
        ToggleButton mNSDToggleButtonView = (ToggleButton) view.findViewById(R.id.nsd_toggle_btn);

        mNSDLabelView.setEnabled(false);
        mNSDToggleButtonView.setEnabled(false);
        mNSDUnsupportedView.setVisibility(View.VISIBLE);
    }

    private void processCustomAppIdCheck(View view, boolean isChecked) {
        EditText appIdView = (EditText) view.findViewById(R.id.selectprotocol_appId);

        appIdView.setEnabled(isChecked);
        AppPreferencesManager.setIsCustomAppId(isChecked);

        if (isChecked) {
            appIdView.setText(AppPreferencesManager.getCustomAppId());
        } else {
            appIdView.setText(AppIdManager.getAppIdByTransport(AppPreferencesManager.getTransportType()));
        }
    }
}