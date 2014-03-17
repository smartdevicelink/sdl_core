package com.ford.syncV4.android.manager;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 17.03.14
 * Time: 14:15
 */

import com.ford.syncV4.android.constants.FlavorConst;
import com.ford.syncV4.transport.TransportType;

/**
 * This class provides AppId value management functionality
 */
public class AppIdManager {

    public static String getAppIdByTransport(TransportType transportType) {
        switch (transportType) {
            case USB:
                return FlavorConst.APPID_USB;
            case BLUETOOTH:
                return FlavorConst.APPID_BT;
            default:
                return FlavorConst.APPID_TCP;
        }
    }
}