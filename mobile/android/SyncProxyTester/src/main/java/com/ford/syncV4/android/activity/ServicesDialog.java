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

import com.ford.syncV4.android.R;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 25.03.14
 * Time: 17:11
 */
public class ServicesDialog extends DialogFragment {

    private static final String LOG_TAG = "ServicesDialog";

    public static ServicesDialog newInstance() {
        ServicesDialog servicesDialog = new ServicesDialog();
        return servicesDialog;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Context mContext = getActivity();
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        View layout = inflater.inflate(R.layout.services_layout,
                (ViewGroup) getActivity().findViewById(R.id.itemRoot));

        // ((SyncProxyTester) getActivity()).getImageTypeAdapter();

        return new AlertDialog.Builder(mContext)
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                }).setView(layout).show();

    }
}