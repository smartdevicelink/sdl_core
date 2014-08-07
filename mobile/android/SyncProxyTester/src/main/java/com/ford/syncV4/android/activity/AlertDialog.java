package com.ford.syncV4.android.activity;

import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.TTSChunkFactory;
import com.ford.syncV4.proxy.rpc.Alert;
import com.ford.syncV4.proxy.rpc.SoftButton;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.enums.SoftButtonType;
import com.ford.syncV4.proxy.rpc.enums.SystemAction;
import com.ford.syncV4.util.logger.Logger;

import java.util.Vector;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 05.08.14
 * Time: 9:50
 */
public class AlertDialog extends BaseDialogFragment {

    private static final String LOG_TAG = AlertDialog.class.getSimpleName();

    /**
     * START
     * These field must be refactored with
     * https://adc.luxoft.com/jira/browse/APPLINK-8099
     */

    /**
     * The Include Soft Buttons checkbox in the current dialog. Kept here to
     * check it when the user has explicitly set the soft buttons.
     */
    private CheckBox chkIncludeSoftButtons;

    /**
     * List of soft buttons for current function. Passed between
     * {@link SoftButtonsListActivity} and this activity.
     */
    private Vector<SoftButton> currentSoftButtons;

    private static final int ALERT_MAXSOFTBUTTONS = 4;

    /**
     * END
     */

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Context context = getActivity();
        LayoutInflater inflater = (LayoutInflater) context.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        View layout = inflater.inflate(R.layout.alert,
                (ViewGroup) getActivity().findViewById(R.id.itemRoot));

        final EditText txtSpeak = (EditText) layout.findViewById(R.id.txtSpeak);
        final EditText txtAlertField1 = (EditText) layout.findViewById(R.id.txtAlertField1);
        final EditText txtAlertField2 = (EditText) layout.findViewById(R.id.txtAlertField2);
        final EditText txtAlertField3 = (EditText) layout.findViewById(R.id.txtAlertField3);
        final EditText txtDuration = (EditText) layout.findViewById(R.id.txtDuration);
        final CheckBox chkPlayTone = (CheckBox) layout.findViewById(R.id.chkPlayTone);
        final CheckBox useProgressIndicator = (CheckBox) layout.findViewById(R.id.alert_useProgressIndicator);
        final CheckBox useDuration = (CheckBox) layout.findViewById(R.id.alert_useDuration);
        final CheckBox doEncryptView = (CheckBox) layout.findViewById(R.id.alert_do_encrypt_view);
        chkIncludeSoftButtons = (CheckBox) layout.findViewById(R.id.chkIncludeSBs);

        SoftButton sb1 = new SoftButton();
        sb1.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
        sb1.setText("ReRoute");
        sb1.setType(SoftButtonType.SBT_TEXT);
        sb1.setIsHighlighted(false);
        sb1.setSystemAction(SystemAction.STEAL_FOCUS);
        SoftButton sb2 = new SoftButton();
        sb2.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
        sb2.setText("Close");
        sb2.setType(SoftButtonType.SBT_TEXT);
        sb2.setIsHighlighted(false);
        sb2.setSystemAction(SystemAction.DEFAULT_ACTION);
        currentSoftButtons = new Vector<SoftButton>();
        currentSoftButtons.add(sb1);
        currentSoftButtons.add(sb2);

        Button btnSoftButtons = (Button) layout.findViewById(R.id.alert_btnSoftButtons);
        btnSoftButtons.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                IntentHelper.addObjectForKey(currentSoftButtons, Const.INTENTHELPER_KEY_OBJECTSLIST);
                Intent intent = new Intent(getActivity(), SoftButtonsListActivity.class);
                intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER, ALERT_MAXSOFTBUTTONS);
                startActivityForResult(intent, PlaceholderFragment.REQUEST_LIST_SOFTBUTTONS);
            }
        });

        return new android.app.AlertDialog.Builder(context)
                .setTitle("RPC Request - Alert")
                .setPositiveButton(getString(R.string.ok_string),
                        new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        try {
                            Alert alert = RPCRequestFactory.buildAlert();
                            alert.setCorrelationId(((SyncProxyTester) getActivity())
                                    .getNextCorrelationIdForCurrentFragment());
                            alert.setAlertText1(txtAlertField1.getText().toString());
                            alert.setAlertText2(txtAlertField2.getText().toString());
                            alert.setAlertText3(txtAlertField3.getText().toString());
                            if (useDuration.isChecked()) {
                                alert.setDuration(Integer.parseInt(txtDuration.getText().toString()));
                            }
                            alert.setPlayTone(chkPlayTone.isChecked());
                            alert.setProgressIndicator(useProgressIndicator.isChecked());

                            String toSpeak = txtSpeak.getText().toString();
                            if (toSpeak.length() > 0) {
                                Vector<TTSChunk> ttsChunks = TTSChunkFactory
                                        .createSimpleTTSChunks(toSpeak);
                                alert.setTtsChunks(ttsChunks);
                            }
                            alert.setDoEncryption(doEncryptView.isChecked());
                            if (chkIncludeSoftButtons.isChecked() &&
                                    (currentSoftButtons != null) &&
                                    (currentSoftButtons.size() > 0)) {
                                alert.setSoftButtons(currentSoftButtons);
                            }
                            ((SyncProxyTester) getActivity()).onAlertDialogResult(
                                    getArguments().getString(APP_ID_KEY), alert);
                        } catch (NumberFormatException e) {
                            SafeToast.showToastAnyThread("Couldn't parse number");
                        }
                        currentSoftButtons = null;
                        chkIncludeSoftButtons = null;
                    }
                })
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        currentSoftButtons = null;
                        chkIncludeSoftButtons = null;
                        dialog.cancel();
                    }
                }).setView(layout).show();
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        Logger.i(LOG_TAG + " OnActivityResult, request:" + requestCode + ", result:" + resultCode +
                ", data:" + data);

        switch (requestCode) {
            case PlaceholderFragment.REQUEST_LIST_SOFTBUTTONS:
                if (resultCode == Activity.RESULT_OK) {
                    currentSoftButtons = (Vector<SoftButton>) IntentHelper.
                            getObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);

                    if (chkIncludeSoftButtons != null) {
                        chkIncludeSoftButtons.setChecked(true);
                    }
                }
                IntentHelper.removeObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);
                break;
        }
    }
}