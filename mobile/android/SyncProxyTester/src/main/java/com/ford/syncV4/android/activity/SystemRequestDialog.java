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
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.Spinner;

import com.ford.syncV4.android.R;
import com.ford.syncV4.proxy.rpc.SystemRequest;
import com.ford.syncV4.proxy.rpc.enums.RequestType;

/**
 * Created by Andrew Batutin on 3/6/14.
 */
public class SystemRequestDialog extends DialogFragment

{

    private static final String LOG_TAG = "SystemRequestDialogTag";

    private static int sItemCmdId = 1;

    public static SystemRequestDialog newInstance() {
        SystemRequestDialog sendAddCommandDialog = new SystemRequestDialog();
        return sendAddCommandDialog;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Context mContext = getActivity();
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        View layout = inflater.inflate(R.layout.systemrequest,
                (ViewGroup) getActivity().findViewById(R.id.itemRoot));

        final CheckBox checkBox = (CheckBox) layout.findViewById(R.id.use_requestType);
        final Spinner spinner = (Spinner) layout.findViewById(R.id.requestType);
        String[] data = getResources().getStringArray(R.array.request_type);
        ArrayAdapter requestTypeAdapter = new ArrayAdapter<String>(this.getActivity(), android.R.layout.select_dialog_item, data);
        requestTypeAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(requestTypeAdapter);
        spinner.setSelection(0);

        return new AlertDialog.Builder(mContext)
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        SystemRequest systemRequest = new SystemRequest();
                        if (checkBox.isChecked()) {
                            systemRequest.setRequestType(RequestType.valueForString((String) spinner.getSelectedItem()));
                        }
                        ((SyncProxyTester) getActivity()).onSystemRequestDialogResult(systemRequest);
                    }
                })
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                }).setView(layout).show();
    }
}