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
import android.widget.Button;
import android.widget.EditText;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.android.R;
import com.ford.syncV4.android.service.email.MailInfo;
import com.ford.syncV4.android.service.email.SendLogEmailTask;
import com.ford.syncV4.android.utils.AppUtils;
import com.ford.syncV4.util.logger.Logger;

import java.io.IOException;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 03.04.14
 * Time: 16:50
 */
public class FeedbackDialog extends DialogFragment {

    public static FeedbackDialog newInstance() {
        FeedbackDialog feedbackDialog = new FeedbackDialog();
        return feedbackDialog;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        Context context = getActivity();
        LayoutInflater inflater = (LayoutInflater) context.getSystemService(
                getActivity().LAYOUT_INFLATER_SERVICE);
        final View view = inflater.inflate(R.layout.feedback_dialog_layout,
                (ViewGroup) getActivity().findViewById(R.id.selectprotocol_Root));

        Button sendLogs = (Button) view.findViewById(R.id.feedback_send_log_btn_view);
        Button zipLogs = (Button) view.findViewById(R.id.feedback_zip_log_btn_view);
        Button clearLogs = (Button) view.findViewById(R.id.feedback_clear_log_btn_view);
        final EditText emailView = (EditText) view.findViewById(R.id.feedback_email_view);

        sendLogs.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String email = emailView.getText().toString();
                if (email == null || (email != null && email.isEmpty())) {
                    SafeToast.showToastAnyThread("E-Mail is not specified");
                    return;
                }
                SendLogEmailTask sendLogMailTask = new SendLogEmailTask();
                sendLogMailTask.execute(new MailInfo(email, "Feedback from Test department",
                        "Logs"));
            }
        });

        zipLogs.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    Logger.zipLogFiles(MainApp.getInstance(), AppUtils.getAppAdditionalSettings());
                } catch (IOException e) {
                    Logger.e("Can not ZIP Logs");
                }
                SafeToast.showToastAnyThread("Log files are ZIPed successfully");
            }
        });

        clearLogs.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Logger.removeAllLogs(MainApp.getInstance());
            }
        });

        return new AlertDialog.Builder(context)
                .setTitle("Feedback")
                .setPositiveButton("Close", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dismiss();
                    }
                })
                .setView(view).show();
    }
}