package com.ford.syncV4.protocol.enums;

import android.test.AndroidTestCase;

import java.util.Vector;

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
        Vector sessionTypes = ServiceType.getList();
        assertTrue(sessionTypes.contains(ServiceType.Mobile_Nav));
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
        Vector sessionTypes = ServiceType.getList();
        assertTrue(sessionTypes.contains(ServiceType.Audio_Service));
    }


}
