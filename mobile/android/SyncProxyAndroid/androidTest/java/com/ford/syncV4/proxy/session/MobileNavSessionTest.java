package com.ford.syncV4.proxy.session;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.session.SessionTest;

import org.mockito.Mockito;

import static org.mockito.Mockito.mock;

/**
 * Created by Andrew Batutin on 8/20/13
 */
public class MobileNavSessionTest extends InstrumentationTestCase {

    private MobileNavSession mMobileNavSession;
    private WiProProtocol mWiProProtocol;

    public MobileNavSessionTest() {

    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        System.setProperty("dexmaker.dexcache", getInstrumentation().getTargetContext()
                .getCacheDir().getPath());
        mWiProProtocol = createMockWiProProtocol();
        mMobileNavSession = new MobileNavSession(mWiProProtocol);
    }

    private WiProProtocol createMockWiProProtocol() {
        WiProProtocol wiProProtocol = mock(WiProProtocol.class);
<<<<<<< HEAD
        Session session = new Session();
        session.setSessionId((byte) 0x01);
        Mockito.doThrow(new IllegalArgumentException("Can't call this method")).when(wiProProtocol).StartProtocolService(ServiceType.Mobile_Nav, session, false);
=======
        Mockito.doThrow(new IllegalArgumentException("Can't call this method"))
                .when(wiProProtocol).StartProtocolService(ServiceType.Mobile_Nav,
                Session.DEFAULT_SESSION_ID);
>>>>>>> develop
        return wiProProtocol;
    }

    public void testMobileNavigationSessionCreation() {
        MobileNavSession mobileNavSessionSession = new MobileNavSession(mWiProProtocol);
        assertNotNull("mobile Nav syncSession should be created", mobileNavSessionSession);
    }

    public void testMobileNavigationStartSession() {
        mMobileNavSession.startService(SessionTest.SESSION_ID);
        assertTrue("should get here",true);
    }
}
