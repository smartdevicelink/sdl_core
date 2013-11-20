package com.ford.avarsdl.util;

import android.annotation.TargetApi;
import android.app.Application;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Environment;
import android.os.Looper;

import java.io.File;

/**
 * Created with IntelliJ IDEA.
 * User: user
 * Date: 11/15/13
 * Time: 12:11 PM
 */
public class AppUtils {

    private static Application appInstance = null;

    public static void setAppInstance(Application appInstance) {
        AppUtils.appInstance = appInstance;
    }

    public static boolean isRunningUIThread() {
        return Looper.myLooper() == Looper.getMainLooper();
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
            //  to know is we can neither read nor write
            mExternalStorageAvailable = mExternalStorageWriteable = false;
        }
        return mExternalStorageAvailable && mExternalStorageWriteable;
    }

    public static String getExternalStorageDir() {
        if (Build.VERSION.SDK_INT < 8) {
            return Environment.getExternalStorageDirectory().getAbsolutePath() + "/RevSDL/" +
                    getPackageInfo().packageName;
        } else {
            File externalDir = getExternalFilesDirAPI8(null);
            return externalDir != null ? externalDir.getAbsolutePath() : null;
        }
    }

    @TargetApi(8)
    private static File getExternalFilesDirAPI8(String type) {
        if (appInstance == null) {
            throw new NullPointerException("RevSDL AppUtils getPackageInfo appInstance is NULL");
        }
        return appInstance.getExternalFilesDir(type);
    }

    /**
     * @return PackageInfo for the current application or null if the PackageManager could not be contacted.
     */
    private static PackageInfo getPackageInfo() {
        if (appInstance == null) {
            throw new NullPointerException("RevSDL AppUtils getPackageInfo appInstance is NULL");
        }
        final PackageManager pm = appInstance.getPackageManager();
        if (pm == null) {
            Logger.w("Package manager is NULL");
            return null;
        }
        String packageName = "";
        try {
            packageName = appInstance.getPackageName();
            return pm.getPackageInfo(packageName, 0);
        } catch (PackageManager.NameNotFoundException e) {
            Logger.e("Failed to find PackageInfo : " + packageName);
            return null;
        } catch (RuntimeException e) {
            // To catch RuntimeException("Package manager has died") that can occur on some version of Android,
            // when the remote PackageManager is unavailable. I suspect this sometimes occurs when the App is being
            // reinstalled.
            Logger.e("Package manager has died : " + packageName);
            return null;
        } catch (Throwable e) {
            Logger.e("Package manager has Throwable : " + e);
            return null;
        }
    }
}