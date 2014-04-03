package com.ford.syncV4.android.activity;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Spinner;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.android.utils.AppUtils;
import com.ford.syncV4.proxy.rpc.SystemRequest;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.lamerman.FileDialog;
import com.lamerman.SelectionMode;

/**
 * Created by Andrew Batutin on 3/6/14
 */
public class SystemRequestDialog extends DialogFragment {

    private EditText mSelectedFileNameView;
    private String mSelectedFilePath = "";

    @SuppressWarnings("unused")
    private static final String LOG_TAG = "SystemRequestDialogTag";

    public static SystemRequestDialog newInstance() {
        SystemRequestDialog systemRequestDialog = new SystemRequestDialog();
        return systemRequestDialog;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Context mContext = getActivity();
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        final View layout = inflater.inflate(R.layout.systemrequest,
                (ViewGroup) getActivity().findViewById(R.id.itemRoot));

        final CheckBox useRequestTypeCheckBox = (CheckBox) layout.findViewById(R.id.use_requestType);
        final CheckBox useFileNameCheckBox =
                (CheckBox) layout.findViewById(R.id.system_request_use_file_name_check_box);
        final Spinner requestTypesSpinner = (Spinner) layout.findViewById(R.id.requestType);
        String[] requestsTypesData = getResources().getStringArray(R.array.request_type);
        ArrayAdapter requestTypeAdapter = new ArrayAdapter<String>(
                getActivity(), android.R.layout.select_dialog_item, requestsTypesData);
        requestTypeAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        requestTypesSpinner.setAdapter(requestTypeAdapter);
        requestTypesSpinner.setSelection(0);

        useRequestTypeCheckBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                requestTypesSpinner.setEnabled(isChecked);
            }
        });

        useFileNameCheckBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                processUseFileNameView(layout, isChecked);
            }
        });

        mSelectedFileNameView = (EditText) layout.findViewById(R.id.system_request_local_file_name);

        Button selectFileView = (Button) layout.findViewById(R.id.system_request_select_file_button);
        selectFileView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // show Choose File dialog
                Intent intent = new Intent(mContext, FileDialog.class);
                intent.putExtra(FileDialog.START_PATH,
                        Environment.getExternalStorageDirectory().getPath());
                intent.putExtra(FileDialog.CAN_SELECT_DIR, false);
                intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_OPEN);
                startActivityForResult(intent, Const.REQUEST_SYSTEM_REQUEST_FILE_OPEN);
            }
        });

        return new AlertDialog.Builder(mContext)
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        SystemRequest systemRequest = new SystemRequest();
                        if (useRequestTypeCheckBox.isChecked()) {
                            systemRequest.setRequestType(
                                    RequestType.valueForString((String) requestTypesSpinner.getSelectedItem()));
                        }
                        if (useFileNameCheckBox.isChecked()) {
                            systemRequest.setFileName(mSelectedFileNameView.getText().toString());
                            byte[] data = AppUtils.contentsOfResource(mSelectedFilePath);
                            systemRequest.setBulkData(data);
                            if (data != null) {
                                Log.d(LOG_TAG, "SystemRequest data length:" + data.length);
                            } else {
                                Log.d(LOG_TAG, "SystemRequest data null");
                            }
                        }
                        ((SyncProxyTester) getActivity()).onSystemRequestDialogResult(systemRequest);
                    }
                })
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.dismiss();
                    }
                }).setView(layout).show();
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (resultCode == Activity.RESULT_OK) {
            if (requestCode == Const.REQUEST_SYSTEM_REQUEST_FILE_OPEN) {
                mSelectedFilePath = data.getStringExtra(FileDialog.RESULT_PATH);
                if (mSelectedFileNameView != null) {
                    if (mSelectedFilePath == null) {
                        return;
                    }
                    String fileName = "";
                    int lastIndexOf = mSelectedFilePath.lastIndexOf("/");
                    if (lastIndexOf == -1) {
                        lastIndexOf = 0;
                    }
                    fileName = mSelectedFilePath.substring(lastIndexOf + 1, mSelectedFilePath.length());
                    mSelectedFileNameView.setText(fileName);
                }
            }
        }
    }

    private void processUseFileNameView(View view, boolean isEnabled) {
        mSelectedFileNameView.setEnabled(isEnabled);
        Button selectFileView = (Button) view.findViewById(R.id.system_request_select_file_button);
        selectFileView.setEnabled(isEnabled);
    }
}