package com.ford.syncV4.protocol.enums;

import android.test.AndroidTestCase;

/**
 * Created by Andrew Batutin on 8/20/13
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
        assertEquals(value, ServiceType.MOBILE_NAV_SERVICE_ID);
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
        assertEquals(value, ServiceType.AUDIO_SERVICE_ID);
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
        assertEquals(value, ServiceType.HEARTBEAT_SERVICE_ID);
    }

    public void testHeartbeatIsInServiceTypeList() throws Exception {
        assertTrue(ServiceType.containsService(ServiceType.Heartbeat));
    }

    public void testServiceTypeSecureNameIsSecure_Service() throws Exception {
        String name = ServiceType.Secure_Service.getName();
        assertEquals(name, "Secure_Service");
    }

    public void testServiceTypeSecureValueIs11() throws Exception {
        byte value = ServiceType.Secure_Service.getValue();
        assertEquals(value, ServiceType.SECURE_SERVICE_ID);
    }

    public void testSecureServiceIsInServiceTypeList() throws Exception {
        assertTrue(ServiceType.containsService(ServiceType.Secure_Service));
    }
}