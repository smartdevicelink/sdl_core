package com.ford.syncV4.proxy.session;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.SessionType;

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
        Mockito.doThrow(new IllegalArgumentException("Can't call this method")).when(wiProProtocol).startProtocolSession(SessionType.Mobile_Nav);
        return wiProProtocol;
    }

    public void testMobileNavigationSessionCreation() throws Exception {
        MobileNavSession mobileNavSessionSession = new MobileNavSession(_protocol);
        assertNotNull("mobile Nav session should be created", mobileNavSessionSession);
    }

    public void testMobileNavigationStartSession() throws Exception {
        _sut.startSession((byte)0x0A);
        assertTrue("should get here",true);
    }
}
