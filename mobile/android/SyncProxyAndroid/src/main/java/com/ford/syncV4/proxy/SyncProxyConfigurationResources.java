package com.ford.syncV4.proxy;

import android.telephony.TelephonyManager;

public class SyncProxyConfigurationResources {

    private String mSyncConfigurationFilePath;
    private TelephonyManager mTelephonyManager;

    public SyncProxyConfigurationResources() {
        this(null, null);
    }

    public SyncProxyConfigurationResources(String syncConfigurationFilePath,
                                           TelephonyManager telephonyManager) {
        mSyncConfigurationFilePath = syncConfigurationFilePath;
        mTelephonyManager = telephonyManager;
    }

    public void setSyncConfigurationFilePath(String syncConfigurationFilePath) {
        mSyncConfigurationFilePath = syncConfigurationFilePath;
    }

    public String getSyncConfigurationFilePath() {
        return mSyncConfigurationFilePath;
    }

    public void setTelephonyManager(TelephonyManager telephonyManager) {
        mTelephonyManager = telephonyManager;
    }

    public TelephonyManager getTelephonyManager() {
        return mTelephonyManager;
    }
}