package com.ford.syncV4.util.logger;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 04.04.14
 * Time: 11:46
 */

import android.content.pm.ApplicationInfo;

/**
 * This class holds additional settings values that describes application that use Android Sync SDK
 */
public class ApplicationAdditionalSettings {

    private ApplicationInfo mApplicationInfo;
    private String mAppName = "undefined";
    private String mAppVersion = "undefined";
    private String mBuildDate = "undefined";

    public ApplicationInfo getApplicationInfo() {
        return mApplicationInfo;
    }

    public void setApplicationInfo(ApplicationInfo mApplicationInfo) {
        this.mApplicationInfo = mApplicationInfo;
    }

    public String getAppName() {
        return mAppName;
    }

    public void setAppName(String mAppName) {
        this.mAppName = mAppName;
    }

    public String getAppVersion() {
        return mAppVersion;
    }

    public void setAppVersion(String mAppVersion) {
        this.mAppVersion = mAppVersion;
    }

    public String getBuildDate() {
        return mBuildDate;
    }

    public void setBuildDate(String mBuildDate) {
        this.mBuildDate = mBuildDate;
    }
}