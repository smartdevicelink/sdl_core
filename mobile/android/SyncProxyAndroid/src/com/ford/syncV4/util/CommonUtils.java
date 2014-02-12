package com.ford.syncV4.util;

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
}