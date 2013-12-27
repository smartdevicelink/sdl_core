package com.ford.syncV4.android.manager;

import android.content.Context;
import android.content.SharedPreferences;

import com.ford.syncV4.android.constants.Const;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 12/27/13
 * Time: 10:51 AM
 */
public class AppPreferencesManager {

    private static Context mAppContext;

    public static void setAppContext(Context aAppContext) {
        if (mAppContext == null) {
            mAppContext = aAppContext;
        }
    }

    /**
     * Get Transport Type that application use.
     * @return value of the Transport Type or -1 if the one undefined
     */
    public static int getTransportType() {
        if (mAppContext == null) {
            return -1;
        }
        SharedPreferences sharedPreferences = mAppContext.getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getInt(Const.Transport.PREFS_KEY_TRANSPORT_TYPE,
                Const.Transport.KEY_UNKNOWN);
    }
}