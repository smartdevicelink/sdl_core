package com.ford.syncV4.proxy.session;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;

import org.mockito.Mockito;

import static org.mockito.Mockito.mock;


/**
 * Created by Andrew Batutin on 8/20/13.
 */
public class MobileNavSessionTest extends InstrumentationTestCase{

    MobileNavSession _sut;
    private WiProProtocol _protocol;

    public MobileNavSessionTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        System.setProperty("dexmaker.dexcache", getInstrumentation().getTargetContext().getCacheDir().getPath());
        _protocol = createMockWiProProtocol();
        _sut = new MobileNavSession(_protocol);
    }

    private WiProProtocol createMockWiProProtocol(){
        WiProProtocol wiProProtocol = mock(WiProProtocol.class);
        Session session = new Session();
        session.setSessionId((byte) 0x01);
        Mockito.doThrow(new IllegalArgumentException("Can't call this method")).when(wiProProtocol).StartProtocolService(ServiceType.Mobile_Nav, session, false);
        return wiProProtocol;
    }

    public void testMobileNavigationSessionCreation() throws Exception {
        MobileNavSession mobileNavSessionSession = new MobileNavSession(_protocol);
        assertNotNull("mobile Nav currentSession should be created", mobileNavSessionSession);
    }

    public void testMobileNavigationStartSession() throws Exception {
        Session session = new Session();
        session.setSessionId((byte)0x0A);
        _sut.startSession(session);
        assertTrue("should get here",true);
    }
}
