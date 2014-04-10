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
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.android.manager.AppPreferencesManager;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.util.logger.Logger;
import com.lamerman.FileDialog;
import com.lamerman.SelectionMode;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/13/14
 * Time: 12:14 PM
 */
public class PolicyFilesSetUpDialog extends DialogFragment {

    private static final String CLASS_NAME = PolicyFilesSetUpDialog.class.getSimpleName();

    private EditText mSelectedPolicyUpdateFileNameView;

    public static PolicyFilesSetUpDialog newInstance() {
        PolicyFilesSetUpDialog dialog = new PolicyFilesSetUpDialog();
        return dialog;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Context mContext = getActivity();
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        View layout = inflater.inflate(R.layout.policy_files_setup_layout, null);

        mSelectedPolicyUpdateFileNameView =
                (EditText) layout.findViewById(R.id.policy_update_local_file_name);

        if (!AppPreferencesManager.getPolicyTableUpdateFilePath().isEmpty()) {
            mSelectedPolicyUpdateFileNameView.setText(
                    AppPreferencesManager.getPolicyTableUpdateFilePath());
        }

        final Button mPolicyUpdateSelectLocalFileView =
                (Button) layout.findViewById(R.id.policy_update_select_file_button);
        mPolicyUpdateSelectLocalFileView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // show Choose File dialog
                Intent intent = new Intent(mContext, FileDialog.class);
                intent.putExtra(FileDialog.START_PATH,
                        Environment.getExternalStorageDirectory().getPath());
                intent.putExtra(FileDialog.CAN_SELECT_DIR, false);
                intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_OPEN);
                startActivityForResult(intent, Const.REQUEST_POLICY_UPDATE_FILE_OPEN);
            }
        });

        final Spinner fileTypeView = (Spinner) layout.findViewById(R.id.send_policy_update_file_type);
        // Create an ArrayAdapter using the string array and a default spinner layout
        final String[] fileTypesArray = getActivity().getResources().getStringArray(R.array.policy_update_file_type);
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(getActivity(),
                R.array.policy_update_file_type, android.R.layout.simple_spinner_item);
        // Specify the layout to use when the list of choices appears
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // Apply the adapter to the spinner
        fileTypeView.setAdapter(adapter);

        Button sendPolicyUpdateView = (Button) layout.findViewById(R.id.send_policy_table_update_btn_view);
        sendPolicyUpdateView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO : Reconsider this check point
                FileType fileType = FileType.JSON;
                String selectedObjectString = String.valueOf(fileTypeView.getSelectedItem());
                if (selectedObjectString == null) {
                    if (selectedObjectString.equalsIgnoreCase(fileTypesArray[0])) {
                        fileType = FileType.JSON;
                    } else if (selectedObjectString.equalsIgnoreCase(fileTypesArray[1])) {
                        fileType = FileType.BINARY;
                    }
                }
                ((SyncProxyTester) getActivity()).onPolicyFilesSetUpDialogResult_SendUpdate(fileType);
            }
        });

        return new AlertDialog.Builder(mContext)
                .setTitle(getString(R.string.policy_files_setup_dialog_title))
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        String mFilePath = mSelectedPolicyUpdateFileNameView.getText().toString();
                        if (mFilePath == null) {
                            mFilePath = "";
                        }
                        AppPreferencesManager.setPolicyTableUpdateFilePath(mFilePath);
                    }
                })
                .setView(layout).show();
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (resultCode == Activity.RESULT_OK) {
            if (requestCode == Const.REQUEST_POLICY_UPDATE_FILE_OPEN) {
                String filePath = data.getStringExtra(FileDialog.RESULT_PATH);
                if (mSelectedPolicyUpdateFileNameView != null) {
                    mSelectedPolicyUpdateFileNameView.setText(filePath);
                }
                AppPreferencesManager.setPolicyTableUpdateFilePath(filePath);
            }
        }
    }
}