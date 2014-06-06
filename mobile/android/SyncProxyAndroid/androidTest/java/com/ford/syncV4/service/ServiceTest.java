package com.ford.syncV4.service;

import android.test.AndroidTestCase;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.session.SessionTest;

/**
 * Created by Andrew Batutin on 1/21/14
 */
public class ServiceTest extends AndroidTestCase {

    public void testServiceWasCreated() throws Exception {
        Service service = new Service();
        assertNotNull(service);
    }

    public void testServiceIdIsSet() throws Exception {
        Session session = SessionTest.getInitializedSession();
        Service service = new Service();
        service.setAppId(SessionTest.APP_ID);
        assertEquals("service id should be 1", session.getAppIdBySessionId(SessionTest.SESSION_ID),
                service.getAppId());
    }

    public void testServiceTypeIsSet() throws Exception {
        Service service = new Service();
        service.setServiceType(ServiceType.RPC);
        ServiceType serviceType = service.getServiceType();
        assertEquals("Service type should be RPC", ServiceType.RPC, serviceType);
    }
}
