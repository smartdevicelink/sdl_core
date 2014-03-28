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
        if (transportType == null) {
            throw new NullPointerException(AppIdManager.class.getSimpleName() +
                    " getAppIdByTransport, transport can not be NULL");
        }
        switch (transportType) {
            case USB:
                return FlavorConst.APPID_USB;
            case BLUETOOTH:
                return FlavorConst.APPID_BT;
            case TCP:
                return FlavorConst.APPID_TCP;
        }
        // Default AppId is an Id of the USB
        return FlavorConst.APPID_USB;
    }
}