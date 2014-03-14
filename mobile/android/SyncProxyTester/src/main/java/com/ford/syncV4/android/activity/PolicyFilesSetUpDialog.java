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
import android.widget.Button;
import android.widget.EditText;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.android.manager.AppPreferencesManager;
import com.lamerman.FileDialog;
import com.lamerman.SelectionMode;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/13/14
 * Time: 12:14 PM
 */
public class PolicyFilesSetUpDialog extends DialogFragment {

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

        Button sendPolicyUpdateView = (Button) layout.findViewById(R.id.send_policy_table_update_btn_view);
        sendPolicyUpdateView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((SyncProxyTester) getActivity()).onPolicyFilesSetUpDialogResult_SendUpdate();
            }
        });

        return new AlertDialog.Builder(mContext)
                .setTitle(getString(R.string.policy_files_setup_dialog_title))
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        String mFilePath = mSelectedPolicyUpdateFileNameView.getText().toString();
                        if (mFilePath == null || mFilePath.equals("")) {
                            return;
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