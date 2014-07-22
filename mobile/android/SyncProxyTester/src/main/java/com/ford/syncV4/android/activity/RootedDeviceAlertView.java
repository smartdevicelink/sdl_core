package com.ford.syncV4.android.activity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 28.03.14
 * Time: 17:26
 */
public class RootedDeviceAlertView extends DialogFragment {

    public static RootedDeviceAlertView newInstance() {
        RootedDeviceAlertView rootedDeviceAlertView = new RootedDeviceAlertView();
        return rootedDeviceAlertView;
    }

    public Dialog onCreateDialog(Bundle savedInstanceState) {
        Context context = getActivity();

        // Use the Builder class for convenient dialog construction
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setMessage("Device on which the program was launched recognized as Rooted. " +
                "Sorry, but the application will be shut down immediately!")
                .setCancelable(false)
                .setTitle("Warning")
                .setPositiveButton("Close Application", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        ((SyncProxyTester) getActivity()).exitApp();
                    }
                });
        // Create the AlertDialog object and return it
        return builder.create();
    }
}