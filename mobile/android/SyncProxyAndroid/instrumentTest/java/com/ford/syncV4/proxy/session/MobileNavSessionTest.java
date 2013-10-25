package com.ford.syncV4.proxy.session;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.protocol.WiProProtocol;

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
        return mock(WiProProtocol.class);
    }

    public void testMobileNavigationSessionCreation() throws Exception {
        MobileNavSession mobileNavSessionSession = new MobileNavSession(_protocol);
        assertNotNull("mobile Nav session should be created", mobileNavSessionSession);
    }

    public void testMobileNavigationStartSession() throws Exception {
        _sut.startSession();
        assertTrue("should get here",true);
    }
}
