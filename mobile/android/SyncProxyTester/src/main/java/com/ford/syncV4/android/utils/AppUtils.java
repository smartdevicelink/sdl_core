package com.ford.syncV4.android.utils;

import android.os.Looper;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 1/30/14
 * Time: 2:15 PM
 */
public class AppUtils {

    public static boolean isRunningUIThread() {
        return Looper.myLooper() == Looper.getMainLooper();
    }
}