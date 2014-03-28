package com.ford.syncV4.util;

import android.content.Context;
import android.telephony.TelephonyManager;
import android.test.InstrumentationTestCase;

import com.ford.syncV4.proxy.rpc.DeviceInfo;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 27.03.14
 * Time: 17:25
 */
public class DeviceInfoManagerTest extends InstrumentationTestCase {

    public void testCreateDeviceInfo() {
        TelephonyManager telephonyManager =
                ((TelephonyManager) getInstrumentation().getContext()
                        .getSystemService(Context.TELEPHONY_SERVICE));

        DeviceInfo deviceInfo = DeviceInfoManager.getDeviceInfo(telephonyManager);

        assertNotNull(deviceInfo);
        assertNotNull(deviceInfo.getHardware());
        assertNotSame("", deviceInfo.getHardware());
        assertNotNull(deviceInfo.getFirmwareRev());
        assertNotSame("", deviceInfo.getFirmwareRev());
        assertNotNull(deviceInfo.getOS());
        assertNotSame("", deviceInfo.getOS());
        assertNotNull(deviceInfo.getOSVersion());
        assertNotSame("", deviceInfo.getOSVersion());
        assertNotNull(deviceInfo.getCarrier());
        assertNotSame("", deviceInfo.getCarrier());
        assertNotNull(deviceInfo.getMaxNumberRFCOMMPorts());
    }

    public void testCopyDeviceInfo() {
        TelephonyManager telephonyManager =
                ((TelephonyManager) getInstrumentation().getContext()
                        .getSystemService(Context.TELEPHONY_SERVICE));

        DeviceInfo deviceInfoCopyFrom = DeviceInfoManager.getDeviceInfo(telephonyManager);

        DeviceInfo deviceInfoCopyTo = new DeviceInfo();

        assertNotNull(deviceInfoCopyTo);
        assertNull(deviceInfoCopyTo.getHardware());
        assertNull(deviceInfoCopyTo.getFirmwareRev());
        assertNull(deviceInfoCopyTo.getOS());
        assertNull(deviceInfoCopyTo.getOSVersion());
        assertNull(deviceInfoCopyTo.getCarrier());
        assertNull(deviceInfoCopyTo.getMaxNumberRFCOMMPorts());

        DeviceInfoManager.copyDeviceInfo(deviceInfoCopyTo, deviceInfoCopyFrom);

        assertNotNull(deviceInfoCopyTo.getHardware());
        assertSame(deviceInfoCopyFrom.getHardware(), deviceInfoCopyTo.getHardware());
        assertNotNull(deviceInfoCopyTo.getFirmwareRev());
        assertSame(deviceInfoCopyFrom.getFirmwareRev(), deviceInfoCopyTo.getFirmwareRev());
        assertNotNull(deviceInfoCopyTo.getOS());
        assertSame(deviceInfoCopyFrom.getOS(), deviceInfoCopyTo.getOS());
        assertNotNull(deviceInfoCopyTo.getOSVersion());
        assertSame(deviceInfoCopyFrom.getOSVersion(), deviceInfoCopyTo.getOSVersion());
        assertNotNull(deviceInfoCopyTo.getCarrier());
        assertSame(deviceInfoCopyFrom.getCarrier(), deviceInfoCopyTo.getCarrier());
        assertNotNull(deviceInfoCopyTo.getMaxNumberRFCOMMPorts());
        assertSame(deviceInfoCopyFrom.getMaxNumberRFCOMMPorts(), deviceInfoCopyTo.getMaxNumberRFCOMMPorts());
    }
}