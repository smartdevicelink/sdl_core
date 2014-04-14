package com.ford.syncV4.util;

import android.content.Context;
import android.os.Environment;

import java.io.File;
import java.util.Arrays;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/12/14
 * Time: 10:12 AM
 */
public class CommonUtils {

    /**
     * This method check whether an error is belongs to well known USB problem at Android OS
     * IOException: write failed: ENODEV (No such device)
     * https://code.google.com/p/android/issues/detail?id=20545
     *
     * @param errorMessage An exception message
     * @return <b>true</b> if provided exception message meet all criteria, <b>false</b> - otherwise
     */
    public static boolean isUSBNoSuchDeviceError(String errorMessage) {
        if (errorMessage == null || errorMessage.equals("")) {
            return false;
        }
        return errorMessage.contains("IOException") && errorMessage.contains("ENODEV") &&
                errorMessage.contains("No such device");
    }

    public static boolean externalStorageAvailable() {
        boolean mExternalStorageAvailable;
        boolean mExternalStorageWriteable;
        String state = Environment.getExternalStorageState();
        if (Environment.MEDIA_MOUNTED.equals(state)) {
            // We can read and write the media
            mExternalStorageAvailable = mExternalStorageWriteable = true;
        } else if (Environment.MEDIA_MOUNTED_READ_ONLY.equals(state)) {
            // We can only read the media
            mExternalStorageAvailable = true;
            mExternalStorageWriteable = false;
        } else {
            // Something else is wrong. It may be one of many other states, but all we need
            // to know is we can neither read nor write
            mExternalStorageAvailable = mExternalStorageWriteable = false;
        }
        return mExternalStorageAvailable && mExternalStorageWriteable;
    }

    public static String getExternalStorageDir(Context context) {
        if (context == null) {
            return null;
        }
        File externalDir = context.getExternalFilesDir(null);
        return externalDir != null ? externalDir.getAbsolutePath() : null;
    }

    public static <T> T[] concat(T[] first, T[] second) {
        T[] result = Arrays.copyOf(first, first.length + second.length);
        System.arraycopy(second, 0, result, first.length, second.length);
        return result;
    }
}