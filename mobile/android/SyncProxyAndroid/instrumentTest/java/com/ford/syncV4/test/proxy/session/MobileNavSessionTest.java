package com.ford.syncV4.test.proxy.session;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.proxy.session.MobileNavSession;

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
    }

    private WiProProtocol createMockWiProProtocol(){
        return mock(WiProProtocol.class);
    }

    public void testMobileNavigationSessionCreation() throws Exception {
        MobileNavSession mobileNavSessionSession = new MobileNavSession(_protocol);
        assertNotNull("mobile Nav session should be created", mobileNavSessionSession);
    }
}
