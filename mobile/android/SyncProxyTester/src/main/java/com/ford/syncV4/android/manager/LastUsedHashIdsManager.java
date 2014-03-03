package com.ford.syncV4.android.manager;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/28/14
 * Time: 1:30 PM
 */

import java.util.LinkedList;

/**
 * This class provides functionality to manage last used
 * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#getHashID()} collection
 */
public class LastUsedHashIdsManager {

    public static final int LAST_USED_IDS_NUMBER = 5;

    private static final String TAG = "LastUsedHashIdsManager";

    private LinkedList<String> mLastUsedHashIds = new LinkedList<String>();

    /**
     * Read collection from the Preferences
     */
    public void init() {
        String lastUsedHashIds = AppPreferencesManager.getLastUsedHashIds();
        String[] strings = lastUsedHashIds.split(",");
        for (String string : strings) {
            mLastUsedHashIds.addLast(string);
        }
    }

    /**
     * Save collection to the Preferences
     */
    public void save() {
        StringBuilder stringBuilder = new StringBuilder();
        for (String mLastUsedHashId : mLastUsedHashIds) {
            stringBuilder.append(mLastUsedHashId);
            stringBuilder.append(",");
        }
        stringBuilder.delete(stringBuilder.length() - 1, stringBuilder.length());
        AppPreferencesManager.setLastUsedHashIds(stringBuilder.toString());
    }

    /**
     * Add new Hash Id to the collection. If the size of the collection will be greater then
     * {@code LAST_USED_IDS_NUMBER}, all other elements will be removed
     *
     * @param value Hash Id
     */
    public void addNewId(String value) {
        while (mLastUsedHashIds.size() >= LAST_USED_IDS_NUMBER) {
            mLastUsedHashIds.pollLast();
        }
        mLastUsedHashIds.addFirst(value);
    }

    /**
     * Return {@link java.lang.String} array of the hash id's
     *
     * @return array of the hash id's
     */
    public String[] getDataForAdapter() {
        String[] strings = new String[mLastUsedHashIds.size()];
        return mLastUsedHashIds.toArray(strings);
    }
}