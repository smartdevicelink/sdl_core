package com.ford.syncV4.android.activity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.android.R;
import com.ford.syncV4.android.manager.AppPreferencesManager;
import com.ford.syncV4.android.manager.LastUsedHashIdsManager;

import java.util.LinkedHashSet;

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

        LastUsedHashIdsManager lastUsedHashIdsManager = MainApp.getInstance().getLastUsedHashIdsManager();
        // Array of choices
        final String lastIds[] = lastUsedHashIdsManager.getDataForAdapter();

        final Spinner lastHashIdsView = (Spinner) layout.findViewById(R.id.hash_id_set_up_ids_spinner_view);
        ArrayAdapter<String> spinnerArrayAdapter = new ArrayAdapter<String>(getActivity(),
                android.R.layout.simple_spinner_item, lastIds);
        // The drop down view
        spinnerArrayAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // Apply the adapter to the spinner
        lastHashIdsView.setAdapter(spinnerArrayAdapter);
        final boolean[] adapterInitWorkaround = {false};
        lastHashIdsView.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                if (adapterInitWorkaround[0]) {
                    setSelectedHashId(layout, lastIds[position]);
                }
                adapterInitWorkaround[0] = true;
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });

        final CheckBox useHashIdView = (CheckBox) layout.findViewById(R.id.hash_id_set_up_use_hash_id_view);
        useHashIdView.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                processUseHashIdCheckBoxEvent(layout, isChecked);
            }
        });
        useHashIdView.setChecked(AppPreferencesManager.getUseHashId());
        processUseHashIdCheckBoxEvent(layout, AppPreferencesManager.getUseHashId());

        final CheckBox useCustomHashIdView = (CheckBox) layout.findViewById(
                R.id.hash_id_set_up_use_custom_hash_id_view);
        useCustomHashIdView.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                processUseCustomHashIdCheckBoxEvent(layout, isChecked);
            }
        });
        useCustomHashIdView.setChecked(AppPreferencesManager.getUseCustomHashId());
        processUseCustomHashIdCheckBoxEvent(layout, AppPreferencesManager.getUseHashId() &&
                AppPreferencesManager.getUseCustomHashId());

        final EditText customHashIdView = (EditText) layout.findViewById(R.id.hash_id_set_up_custom_id_view);
        Log.d(LOG_TAG, "GetHashId, hashId:" + AppPreferencesManager.getCustomHashId());
        customHashIdView.setText(AppPreferencesManager.getCustomHashId());

        return new AlertDialog.Builder(mContext)
                .setCancelable(false)
                .setTitle("HashId set up")
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        final EditText customHashIdView = (EditText) layout.findViewById(
                                R.id.hash_id_set_up_custom_id_view);
                        Log.d(LOG_TAG, "SetSelectedHashId on click, hashId:" +
                                customHashIdView.getText().toString().trim());
                        AppPreferencesManager.setCustomHashId(
                                customHashIdView.getText().toString().trim());
                    }
                })
                .setView(layout).show();
    }

    private void processUseHashIdCheckBoxEvent(View layout, boolean isChecked) {
        Log.d(LOG_TAG, "ProcessUseHashIdCheckBoxEvent, checked:" + isChecked);

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

        if (isChecked) {
            processUseCustomHashIdCheckBoxEvent(layout, AppPreferencesManager.getUseCustomHashId());
        }
    }

    private void processUseCustomHashIdCheckBoxEvent(View layout, boolean isChecked) {
        Log.d(LOG_TAG, "ProcessUseCustomHashIdCheckBoxEvent, checked:" + isChecked);

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

    private void setSelectedHashId(View layout, String hashId) {
        final EditText customHashIdView = (EditText) layout.findViewById(R.id.hash_id_set_up_custom_id_view);
        customHashIdView.setText(hashId);

        Log.d(LOG_TAG, "SetSelectedHashId, hashId:" + hashId);
        AppPreferencesManager.setCustomHashId(hashId);
    }
}