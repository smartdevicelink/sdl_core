package com.ford.syncV4.android.activity;

import android.content.Context;
import android.widget.Toast;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.android.utils.AppUtils;
import com.ford.syncV4.util.logger.Logger;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 1/30/14
 * Time: 2:11 PM
 */
public class SafeToast {

    public static void showToastAnyThread(CharSequence text) {
        showToastAnyThread(MainApp.getInstance(), text);
    }

    public static void showToastAnyThread(final Context context, final CharSequence text) {
        if (AppUtils.isRunningUIThread()) {
            // we are already in UI thread, it's safe to show Toast
            showToastUIThread(context, text);
        } else {
            // we are NOT in UI thread, so scheduling task in handler
            MainApp.getInstance().runInUIThread(new Runnable() {
                @Override
                public void run() {
                    showToastUIThread(context, text);
                }
            });
        }
    }

    private static void showToastUIThread(Context context, CharSequence text) {
        if (context == null) {
            context = MainApp.getInstance();
        }
        Logger.d("SafeToast", "- Show toast: " + text);
        Toast.makeText(context, text, Toast.LENGTH_LONG).show();
    }
}