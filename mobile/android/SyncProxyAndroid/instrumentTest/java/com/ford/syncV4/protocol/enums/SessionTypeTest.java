package com.ford.syncV4.protocol.enums;

import android.test.AndroidTestCase;

import java.util.Vector;

/**
 * Created by Andrew Batutin on 8/20/13.
 */
public class SessionTypeTest extends AndroidTestCase {

    public SessionTypeTest() {
    }

    public void testSessionTypeMobNavNameIsMobile_Nav() throws Exception {
        String name = SessionType.Mobile_Nav.getName();
        assertEquals(name, "Mobile_Nav");
    }

    public void testSessionTypeMobNavValueIs0x0B() throws Exception {
        byte value = SessionType.Mobile_Nav.getValue();
        assertEquals(value, (byte) 0x0B);
    }

    public void testMobileNavIsInSessionTypeList() throws Exception {
        Vector sessionTypes = SessionType.getList();
        assertTrue(sessionTypes.contains(SessionType.Mobile_Nav));
    }
}
