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

    /**
     * Set a path to the Policy Table Update file
     *
     * @param filePath path to the local file
     */
    public static void setPolicyTableUpdateFilePath(String filePath) {
        SharedPreferences sharedPreferences = mAppContext.getSharedPreferences(Const.PREFS_NAME, 0);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putString(Const.Policy.PREF_KEY_POLICY_UPDATE_FILE_PATH, filePath);
        editor.commit();
    }

    /**
     * Get a path to the Policy Table Update local file
     *
     * @return path to the file
     */
    public static String getPolicyTableUpdateFilePath() {
        SharedPreferences sharedPreferences = mAppContext.getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getString(Const.Policy.PREF_KEY_POLICY_UPDATE_FILE_PATH, "");
    }

    /**
     * Set a value of the auto replay scenario in case of processing Policy Table Snapshot data
     *
     * @param value {@link java.lang.Boolean} true | false
     */
    public static void setPolicyTableUpdateAutoReplay(boolean value) {
        SharedPreferences sharedPreferences = mAppContext.getSharedPreferences(Const.PREFS_NAME, 0);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putBoolean(Const.Policy.PREF_KEY_POLICY_UPDATE_AUTO_REPLAY, value);
        editor.commit();
    }

    /**
     * Return a value of the auto replay scenario in case of processing Policy Table Snapshot data
     *
     * @return {@link java.lang.Boolean} true | false
     */
    public static boolean getPolicyTableUpdateAutoReplay() {
        SharedPreferences sharedPreferences = mAppContext.getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getBoolean(Const.Policy.PREF_KEY_POLICY_UPDATE_AUTO_REPLAY, true);
    }

    /**
     * Set <b>true</b> if application need to use HashId, <b>false</b> - otherwise
     *
     * @param value {@link java.lang.Boolean} true | false
     */
    public static void setUseHashId(boolean value) {
        SharedPreferences sharedPreferences = mAppContext.getSharedPreferences(Const.PREFS_NAME, 0);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putBoolean(Const.HashId.PREF_KEY_USE_HASH_ID, value);
        editor.commit();
    }

    /**
     * Return <b>true</b> if application need to use HashId, <b>false</b> - otherwise
     *
     * @return {@link java.lang.Boolean} true | false
     */
    public static boolean getUseHashId() {
        SharedPreferences sharedPreferences = mAppContext.getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getBoolean(Const.HashId.PREF_KEY_USE_HASH_ID, true);
    }

    /**
     * Set <b>true</b> if application need to use Custom HashId, <b>false</b> - otherwise
     *
     * @param value {@link java.lang.Boolean} true | false
     */
    public static void setUseCustomHashId(boolean value) {
        SharedPreferences sharedPreferences = mAppContext.getSharedPreferences(Const.PREFS_NAME, 0);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putBoolean(Const.HashId.PREF_KEY_USE_CUSTOM_HASH_ID, value);
        editor.commit();
    }

    /**
     * Return <b>true</b> if application need to use Custom HashId, <b>false</b> - otherwise
     *
     * @return {@link java.lang.Boolean} true | false
     */
    public static boolean getUseCustomHashId() {
        SharedPreferences sharedPreferences = mAppContext.getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getBoolean(Const.HashId.PREF_KEY_USE_CUSTOM_HASH_ID, false);
    }

    /**
     * Set custom {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#setHashID(String)} field value for the
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface}
     *
     * @param value {@link java.lang.String}
     */
    public static void setCustomHashId(String value) {
        SharedPreferences sharedPreferences = mAppContext.getSharedPreferences(Const.PREFS_NAME, 0);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putString(Const.HashId.PREF_KEY_CUSTOM_HASH_ID, value);
        editor.commit();
    }

    /**
     * @return custom {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#setHashID(String)}
     * field value for the {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface}
     */
    public static String getCustomHashId() {
        SharedPreferences sharedPreferences = mAppContext.getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getString(Const.HashId.PREF_KEY_CUSTOM_HASH_ID, "");
    }
}