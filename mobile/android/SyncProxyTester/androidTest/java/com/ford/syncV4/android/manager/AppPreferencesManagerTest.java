package com.ford.syncV4.android.manager;

import com.ford.syncV4.transport.TransportType;

import junit.framework.TestCase;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 17.03.14
 * Time: 16:51
 */
public class AppPreferencesManagerTest extends TestCase {

    public void testSetGetTransportTypeBluetooth() {
        TransportType transportType = TransportType.BLUETOOTH;
        AppPreferencesManager.setTransportType(transportType);
        assertEquals(transportType, AppPreferencesManager.getTransportType());
    }

    public void testSetGetTransportTypeTCP() {
        TransportType transportType = TransportType.TCP;
        AppPreferencesManager.setTransportType(transportType);
        assertEquals(transportType, AppPreferencesManager.getTransportType());
    }

    public void testSetGetTransportTypeUSB() {
        TransportType transportType = TransportType.USB;
        AppPreferencesManager.setTransportType(transportType);
        assertEquals(transportType, AppPreferencesManager.getTransportType());
    }

    public void testSetGetTransportTypeNull() {
        TransportType transportType = null;
        try {
            AppPreferencesManager.setTransportType(transportType);
            fail();
        } catch (NullPointerException e) {
            /* Expected */
        }
    }

    public void testSetGetCustomAppId() {
        String customAppId = "123qweasd";
        AppPreferencesManager.setCustomAppId(customAppId);
        assertEquals(customAppId, AppPreferencesManager.getCustomAppId());
    }

    public void testSetGetCustomAppIdNull() {
        String customAppId = null;
        try {
            AppPreferencesManager.setCustomAppId(customAppId);
        } catch (NullPointerException e) {
            /* expected */
        }
    }

    public void testSetGetIsCustomAppId() {
        boolean isCustomAppId = true;
        AppPreferencesManager.setIsCustomAppId(isCustomAppId);
        assertEquals(isCustomAppId, AppPreferencesManager.getIsCustomAppId());
    }
}