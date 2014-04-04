package com.ford.syncV4.android.service.email;

import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.android.activity.SafeToast;
import com.ford.syncV4.android.utils.AppUtils;
import com.ford.syncV4.util.logger.Logger;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 04.04.14
 * Time: 9:47
 */
public class SendLogEmailTask extends AsyncTask<MailInfo, Void, Intent> {

    @Override
    protected void onPreExecute() {

    }

    @Override
    protected Intent doInBackground(MailInfo... mailInfoArray) {
        if (mailInfoArray == null) {
            throw new NullPointerException("mailInfoArray");
        }
        if (mailInfoArray.length != 1) {
            throw new IllegalArgumentException("mailInfo");
        }
        MailInfo mailInfo = mailInfoArray[0];
        if (mailInfo == null) {
            throw new NullPointerException("mailInfo");
        }

        // Prepare mail intent
        Intent sendIntent = new Intent(Intent.ACTION_SEND_MULTIPLE);
        sendIntent.putExtra(Intent.EXTRA_EMAIL, new String[]{mailInfo.getTo()});
        sendIntent.putExtra(Intent.EXTRA_SUBJECT, mailInfo.getSubj());
        sendIntent.setType("text/plain");

        try {
            ArrayList<Uri> uris = new ArrayList<Uri>();
            String logFileUrl = Logger.zipLogFiles(MainApp.getInstance(),
                    AppUtils.getAppAdditionalSettings());
            File fileIn = new File(logFileUrl);
            Uri u = Uri.fromFile(fileIn);
            uris.add(u);
            sendIntent.putParcelableArrayListExtra(Intent.EXTRA_STREAM, uris);
            //sendIntent.putExtra(Intent.EXTRA_TEXT, mailInfo.getMailBody() + "\r\n" + logFileUrl);
        } catch (IOException e) {
            return null;
        } catch (Exception e) {
            Logger.e("Unable to send the Log file", e);
            return null;
        }

        return sendIntent;
    }

    @Override
    protected void onPostExecute(Intent sendIntent) {
        if (sendIntent != null) {
            try {
                Intent intent = Intent.createChooser(sendIntent, "Send mail:");
                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                MainApp.getInstance().startActivity(intent);
            } catch (ActivityNotFoundException e) {
                SafeToast.showToastAnyThread("Can not start Mail activity");
                Logger.e("Can not start Mail activity:", e);
            }
            //Logger.i("Logs was sent");
            //SafeToast.showToastAnyThread("Logs was sent");
        } else {
            SafeToast.showToastAnyThread("Failed to send Log file");
        }
    }
}