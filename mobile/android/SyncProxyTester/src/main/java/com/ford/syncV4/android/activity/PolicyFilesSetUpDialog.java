package com.ford.syncV4.android.activity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;

import com.ford.syncV4.android.R;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/13/14
 * Time: 12:14 PM
 */
public class PolicyFilesSetUpDialog extends DialogFragment {

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

        return new AlertDialog.Builder(mContext)
                .setTitle(getString(R.string.policy_files_setup_dialog_title))
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        
                    }
                })
                .setView(layout).show();
    }
}