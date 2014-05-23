package com.ford.syncV4.service;

import android.test.AndroidTestCase;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;

/**
 * Created by Andrew Batutin on 1/21/14.
 */
public class ServiceTest extends AndroidTestCase {

    public void testServiceWasCreated() throws Exception {
        Service service = new Service();
        assertNotNull(service);
    }

    public void testServiceIdIsSet() throws Exception {
        Service service = new Service();
        Session session = new Session();
        service.setSessionId(Session.DEFAULT_SESSION_ID);
        assertEquals("service id should be 1", session, service.getSessionId());
    }

    public void testServiceTypeIsSet() throws Exception {
        Service service = new Service();
        service.setServiceType(ServiceType.RPC);
        ServiceType serviceType = service.getServiceType();
        assertEquals("Service type should be RPC", ServiceType.RPC, serviceType);
    }
}
