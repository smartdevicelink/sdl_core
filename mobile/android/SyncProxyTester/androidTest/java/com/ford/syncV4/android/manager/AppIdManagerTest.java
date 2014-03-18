package com.ford.syncV4.android.manager;

import com.ford.syncV4.android.constants.FlavorConst;
import com.ford.syncV4.transport.TransportType;

import junit.framework.TestCase;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 17.03.14
 * Time: 17:12
 */
public class AppIdManagerTest extends TestCase {

    public void testGetUSBId() {
        assertEquals(FlavorConst.APPID_USB, AppIdManager.getAppIdByTransport(TransportType.USB));
    }

    public void testGetTCPId() {
        assertEquals(FlavorConst.APPID_TCP, AppIdManager.getAppIdByTransport(TransportType.TCP));
    }

    public void testGetBluetoothId() {
        assertEquals(FlavorConst.APPID_BT, AppIdManager.getAppIdByTransport(TransportType.BLUETOOTH));
    }

    public void testGetDefaultId() {
        try {
            assertEquals(FlavorConst.APPID_USB, AppIdManager.getAppIdByTransport(null));
        } catch (NullPointerException e) {
            /* Expected */
        }
    }
}