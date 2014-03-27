package com.ford.syncV4.util;

import android.os.Build;
import android.telephony.TelephonyManager;
import android.util.Log;

import com.ford.syncV4.proxy.rpc.DeviceInfo;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 27.03.14
 * Time: 14:00
 */

/**
 * This class provide functionality to create and manage API object -
 * {@link com.ford.syncV4.proxy.rpc.DeviceInfo}
 */
public class DeviceInfoManager {

    private static final String LOG_TAG = "DeviceInfoManager";
    private static TelephonyManager sTelephonyManager;

    /**
     * Create and return {@link com.ford.syncV4.proxy.rpc.DeviceInfo} object
     * @param telephonyManager {@link android.telephony.TelephonyManager} object
     * @return {@link com.ford.syncV4.proxy.rpc.DeviceInfo} object
     */
    public static DeviceInfo getDeviceInfo(TelephonyManager telephonyManager) {
        sTelephonyManager = telephonyManager;

        DeviceInfo deviceInfo = new DeviceInfo();
        deviceInfo.setHardware(getDeviceModel());
        deviceInfo.setFirmwareRev(getFirmwareVersion());
        deviceInfo.setOS("Android");
        deviceInfo.setOSVersion(getOSVersion());
        deviceInfo.setCarrier(getCarrier());
        // TODO : Fake value
        deviceInfo.setMaxNumberRFCOMMPorts(1);
        return deviceInfo;
    }

    /**
     * Copies all fields values from one object to another
     * @param copyTo {@link com.ford.syncV4.proxy.rpc.DeviceInfo} object to copy to
     * @param copyFrom {@link com.ford.syncV4.proxy.rpc.DeviceInfo} object to copy from
     */
    public static void copyDeviceInfo(DeviceInfo copyTo, DeviceInfo copyFrom) {
        if (copyFrom == null && copyTo == null) {
            return;
        }
        copyTo.setHardware(copyFrom.getHardware());
        copyTo.setFirmwareRev(copyFrom.getFirmwareRev());
        copyTo.setOS(copyFrom.getOS());
        copyTo.setOSVersion(copyFrom.getOSVersion());
        copyTo.setCarrier(copyFrom.getCarrier());
        copyTo.setMaxNumberRFCOMMPorts(copyFrom.getMaxNumberRFCOMMPorts());
    }

    private static String getOSVersion() {
        return Build.VERSION.CODENAME;
    }

    private static String getFirmwareVersion() {
        return "Firmware: " + Build.VERSION.RELEASE + ", Kernel:" +
                System.getProperty("os.version");
    }

    private static String getDeviceModel() {
        String manufacturer = Build.MANUFACTURER;
        String model = Build.MODEL;
        if (model.startsWith(manufacturer)) {
            return capitalize(model);
        } else {
            return capitalize(manufacturer) + " " + model;
        }
    }

    private static String capitalize(String s) {
        if (s == null || s.length() == 0) {
            return "";
        }
        char first = s.charAt(0);
        if (Character.isUpperCase(first)) {
            return s;
        } else {
            return Character.toUpperCase(first) + s.substring(1);
        }
    }

    private static String getCarrier() {
        if (!getDeviceNetworkOperatorName().isEmpty()) {
            return getDeviceNetworkOperatorName();
        }
        return getDeviceNetworkOperator();
    }

    private static String getDeviceNetworkOperator() {
        String value = "";
        if (sTelephonyManager != null) {
            try {
                value = sTelephonyManager.getNetworkOperator();
            } catch (Throwable throwable) {
                Log.e(LOG_TAG, " getDeviceNetworkOperator error", throwable);
            }
        }
        return value;
    }

    private static String getDeviceNetworkOperatorName() {
        String value = "";
        if (sTelephonyManager != null) {
            try {
                value = sTelephonyManager.getNetworkOperatorName();
            } catch (Throwable throwable) {
                Log.e(LOG_TAG, " getDeviceNetworkOperatorName error", throwable);
            }
        }
        return value;
    }
}