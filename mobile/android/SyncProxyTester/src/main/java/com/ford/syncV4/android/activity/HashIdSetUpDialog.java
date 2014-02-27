package com.ford.syncV4.android.activity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.manager.AppPreferencesManager;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/26/14
 * Time: 4:46 PM
 */
public class HashIdSetUpDialog extends DialogFragment {

    private static final String LOG_TAG = "HashIdSetUpDialog";

    public static HashIdSetUpDialog newInstance() {
        HashIdSetUpDialog hashIdSetUpDialog = new HashIdSetUpDialog();
        return hashIdSetUpDialog;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Context mContext = getActivity();
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        final View layout = inflater.inflate(R.layout.hash_id_setup_dialog_layout,
                (ViewGroup) getActivity().findViewById(R.id.itemRoot));

        final CheckBox useHashIdView = (CheckBox) layout.findViewById(R.id.hash_id_set_up_use_hash_id_view);
        useHashIdView.setChecked(AppPreferencesManager.getUseHashId());
        useHashIdView.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                processUseHashIdCheckBoxEvent(layout, isChecked);
            }
        });

        final CheckBox useCustomHashIdView = (CheckBox) layout.findViewById(
                R.id.hash_id_set_up_use_custom_hash_id_view);
        useCustomHashIdView.setChecked(AppPreferencesManager.getUseCustomHashId());
        useCustomHashIdView.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                processUseCustomHashIdCheckBoxEvent(layout, isChecked);
            }
        });

        return new AlertDialog.Builder(mContext)
                .setTitle("HashId set up")
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        final EditText customHashIdView = (EditText) layout.findViewById(
                                R.id.hash_id_set_up_custom_id_view);
                        AppPreferencesManager.setCustomHashId(
                                customHashIdView.getText().toString().trim());
                    }
                })
                .setView(layout).show();
    }

    private void processUseHashIdCheckBoxEvent(View layout, boolean isChecked) {
        final TextView customHashIdLabelView = (TextView) layout.findViewById(R.id.hash_id_set_up_custom_id_label_view);
        final TextView lastHashIdsLabelView = (TextView) layout.findViewById(R.id.hash_id_set_up_last_ids_label_view);
        final EditText customHashIdView = (EditText) layout.findViewById(R.id.hash_id_set_up_custom_id_view);
        final Spinner lastHashIdsView = (Spinner) layout.findViewById(R.id.hash_id_set_up_ids_spinner_view);
        final CheckBox useCustomHashIdView = (CheckBox) layout.findViewById(R.id.hash_id_set_up_use_custom_hash_id_view);

        customHashIdLabelView.setEnabled(isChecked);
        lastHashIdsLabelView.setEnabled(isChecked);
        customHashIdView.setEnabled(isChecked);
        lastHashIdsView.setEnabled(isChecked);
        useCustomHashIdView.setEnabled(isChecked);

        AppPreferencesManager.setUseHashId(isChecked);
    }

    private void processUseCustomHashIdCheckBoxEvent(View layout, boolean isChecked) {
        final TextView customHashIdLabelView = (TextView) layout.findViewById(R.id.hash_id_set_up_custom_id_label_view);
        final TextView lastHashIdsLabelView = (TextView) layout.findViewById(R.id.hash_id_set_up_last_ids_label_view);
        final EditText customHashIdView = (EditText) layout.findViewById(R.id.hash_id_set_up_custom_id_view);
        final Spinner lastHashIdsView = (Spinner) layout.findViewById(R.id.hash_id_set_up_ids_spinner_view);

        customHashIdLabelView.setEnabled(isChecked);
        lastHashIdsLabelView.setEnabled(isChecked);
        customHashIdView.setEnabled(isChecked);
        lastHashIdsView.setEnabled(isChecked);

        AppPreferencesManager.setUseCustomHashId(isChecked);
    }
}