package com.ford.avarsdl.activity;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;
import yuriy.chernyshov.yakimbi.business.MainApp;
import yuriy.chernyshov.yakimbi.utils.AppUtils;

/**
 * Created with IntelliJ IDEA.
 * User: ChernyshovYuriy
 * Date: 12.02.13
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
        Log.d(MainApp.APP_TAG, "- Show toast: " + text);
        Toast.makeText(context, text, Toast.LENGTH_LONG).show();
    }
}