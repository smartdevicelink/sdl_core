package com.ford.syncV4.protocol.enums;

import android.test.AndroidTestCase;

/**
 * Created by Andrew Batutin on 8/20/13.
 */
public class ServiceTypeTest extends AndroidTestCase {

    public ServiceTypeTest() {

    }

    public void testServiceTypeMobNavNameIsMobile_Nav() throws Exception {
        String name = ServiceType.Mobile_Nav.getName();
        assertEquals(name, "Mobile_Nav");
    }

    public void testServiceTypeMobNavValueIs0x0B() throws Exception {
        byte value = ServiceType.Mobile_Nav.getValue();
        assertEquals(value, (byte) 0x0B);
    }

    public void testMobileNavIsInServiceTypeList() throws Exception {
        assertTrue(ServiceType.containsService(ServiceType.Mobile_Nav));
    }

    public void testServiceTypeAudioNameIsAudio_Service() throws Exception {
        String name = ServiceType.Audio_Service.getName();
        assertEquals(name, "Audio_Service");
    }

    public void testServiceTypeAudioServiceValueIs10() throws Exception {
        byte value = ServiceType.Audio_Service.getValue();
        assertEquals(value, (byte) 10);
    }

    public void testAudioServiceIsInServiceTypeList() throws Exception {
        assertTrue(ServiceType.containsService(ServiceType.Audio_Service));
    }

    public void testServiceTypeHeartbeatNameIsHeartbeat_Service() throws Exception {
        String name = ServiceType.Heartbeat.getName();
        assertEquals(name, "Heartbeat_Service");
    }

    public void testServiceTypeHeartbeatValueIs0() throws Exception {
        byte value = ServiceType.Heartbeat.getValue();
        assertEquals(value, (byte) 0);
    }

    public void testHeartbeatIsInServiceTypeList() throws Exception {
        assertTrue(ServiceType.containsService(ServiceType.Heartbeat));
    }

    public void testServiceTypeSecureNameIsSecure_Service() throws Exception {
        String name = ServiceType.Secure_Service.getName();
        assertEquals(name, "Secure_Service");
    }

    // TODO : Replace byte value when it will be considered
    public void testServiceTypeSecureValueIs11() throws Exception {
        byte value = ServiceType.Secure_Service.getValue();
        assertEquals(value, (byte) 11);
    }

    public void testSecureServiceIsInServiceTypeList() throws Exception {
        assertTrue(ServiceType.containsService(ServiceType.Secure_Service));
    }
}