package com.ford.syncV4.android.manager;

import android.content.Context;
import android.content.SharedPreferences;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.transport.TransportType;
import com.stericson.RootTools.RootTools;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 12/27/13
 * Time: 10:51 AM
 */
public class AppPreferencesManager {

    /**
     * Set Transport Type that application use.
     * @param transportType Transport Type that application use.
     */
    public static void setTransportType(TransportType transportType) {
        if (transportType == null) {
            throw new NullPointerException(AppPreferencesManager.class.getSimpleName() +
                    " set transport type can not be NULL");
        }
        int transportTypeIntValue = Const.Transport.KEY_USB;
        switch (transportType) {
            case TCP:
                transportTypeIntValue = Const.Transport.KEY_TCP;
                break;
            case BLUETOOTH:
                transportTypeIntValue = Const.Transport.KEY_BLUETOOTH;
                break;
        }
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putInt(Const.Transport.PREFS_KEY_TRANSPORT_TYPE, transportTypeIntValue);
        editor.commit();
    }

    /**
     * @return Transport Type that application use.
     */
    public static TransportType getTransportType() {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        int transportType = sharedPreferences.getInt(Const.Transport.PREFS_KEY_TRANSPORT_TYPE,
                Const.Transport.PREFS_DEFAULT_TRANSPORT_TYPE);
        switch (transportType) {
            case Const.Transport.KEY_BLUETOOTH:
                return TransportType.BLUETOOTH;
            case Const.Transport.KEY_USB:
                return TransportType.USB;
            case Const.Transport.KEY_TCP:
                return TransportType.TCP;
        }
        // Return default transport type
        return TransportType.USB;
    }

    /**
     * Set a path to the Policy Table Update file
     *
     * @param filePath path to the local file
     */
    public static void setPolicyTableUpdateFilePath(String filePath) {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
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
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getString(Const.Policy.PREF_KEY_POLICY_UPDATE_FILE_PATH, "");
    }

    /**
     * Set a value of the auto replay scenario in case of processing Policy Table Snapshot data
     *
     * @param value {@link java.lang.Boolean} true | false
     */
    public static void setPolicyTableUpdateAutoReplay(boolean value) {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
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
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getBoolean(Const.Policy.PREF_KEY_POLICY_UPDATE_AUTO_REPLAY, true);
    }

    /**
     * Set <b>true</b> if application need to use HashId, <b>false</b> - otherwise
     *
     * @param value {@link java.lang.Boolean} true | false
     */
    public static void setUseHashId(boolean value) {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
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
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getBoolean(Const.HashId.PREF_KEY_USE_HASH_ID, true);
    }

    /**
     * Set <b>true</b> if application need to use Custom HashId, <b>false</b> - otherwise
     *
     * @param value {@link java.lang.Boolean} true | false
     */
    public static void setUseCustomHashId(boolean value) {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
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
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getBoolean(Const.HashId.PREF_KEY_USE_CUSTOM_HASH_ID, false);
    }

    /**
     * Set custom {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#setHashID(String)} field
     * value for the {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface}
     *
     * @param value {@link java.lang.String}
     */
    public static void setCustomHashId(String value) {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putString(Const.HashId.PREF_KEY_CUSTOM_HASH_ID, value);
        editor.commit();
    }

    /**
     * @return custom {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#setHashID(String)}
     * field value for the {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface}
     */
    public static String getCustomHashId() {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getString(Const.HashId.PREF_KEY_CUSTOM_HASH_ID, "");
    }

    /**
     * Set last used {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#getHashID()} Set
     *
     * @param value {@link java.util.Set}
     */
    public static void setLastUsedHashIds(String value) {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putString(Const.HashId.PREF_KEY_LAST_HASH_IDS, value);
        editor.commit();
    }

    /**
     * @return last used {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#getHashID()} Set
     */
    public static String getLastUsedHashIds() {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getString(Const.HashId.PREF_KEY_LAST_HASH_IDS, "");
    }

    /**
     * @return Whether or not application use custom AppId, <b>true</b> | <b>false</b>
     */
    public static boolean getIsCustomAppId() {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getBoolean(Const.PREF_KEY_IS_CUSTOM_APP_ID, false);
    }

    /**
     * Set whether or not application use custom AppId, <b>true</b> | <b>false</b>
     * @param value a boolean value
     */
    public static void setIsCustomAppId(boolean value) {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putBoolean(Const.PREF_KEY_IS_CUSTOM_APP_ID, value);
        editor.commit();
    }

    /**
     * Set custom App Id value
     *
     * @param value String value
     */
    public static void setCustomAppId(String value) {
        if (value == null) {
            throw new NullPointerException(AppPreferencesManager.class.getSimpleName() +
                    " set custom App Id can not be NULL");
        }
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putString(Const.PREF_KEY_CUSTOM_APP_ID, value);
        editor.commit();
    }

    /**
     * @return custom App Id value
     */
    public static String getCustomAppId() {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getString(Const.PREF_KEY_CUSTOM_APP_ID, "");
    }

    /**
     * Returns the current state of the disable lock when testing flag.
     *
     * @return true if the screen lock is disabled
     */
    public static boolean getDisableLockFlag() {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getBoolean(
                Const.PREFS_KEY_DISABLE_LOCK_WHEN_TESTING,
                Const.PREFS_DEFAULT_DISABLE_LOCK_WHEN_TESTING);
    }

    /**
     * Toggles the current state of the disable lock when testing flag, and
     * writes it to the preferences.
     */
    public static void toggleDisableLock() {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        boolean disableLock = sharedPreferences.getBoolean(
                Const.PREFS_KEY_DISABLE_LOCK_WHEN_TESTING,
                Const.PREFS_DEFAULT_DISABLE_LOCK_WHEN_TESTING);
        disableLock = !disableLock;
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putBoolean(Const.PREFS_KEY_DISABLE_LOCK_WHEN_TESTING, disableLock);
        editor.commit();
    }

    /**
<<<<<<< HEAD
     * @return true if it is necessary to check whether device has been rooted
     */
    public static boolean getDoDeviceRootCheck() {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getBoolean(Const.PREF_KEY_DO_DEVICE_ROOT_CHECK, true);
    }

    /**
     * Set true if it is necessary to check whether device has been rooted
     * @param value boolean value
     */
    public static void setDoDeviceRootCheck(boolean value) {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putBoolean(Const.PREF_KEY_DO_DEVICE_ROOT_CHECK, value);
        editor.commit();
    }

    /**
     * Set protocol version int value
     * @param value version of the protocol
     */
    public static void setProtocolVersion(int value) {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putInt(Const.PREFS_KEY_PROTOCOL_VERSION, value);
        editor.commit();
    }

    /**
     * @return protocol version int value
     */
    public static int getProtocolVersion() {
        SharedPreferences sharedPreferences = getAppContext().getSharedPreferences(Const.PREFS_NAME, 0);
        return sharedPreferences.getInt(Const.PREFS_KEY_PROTOCOL_VERSION, ProtocolConstants.PROTOCOL_VERSION_MIN);
    }

    /**
     * @return the Context of the application
     */
    private static Context getAppContext() {
        return MainApp.getInstance().getApplicationContext();
    }
}