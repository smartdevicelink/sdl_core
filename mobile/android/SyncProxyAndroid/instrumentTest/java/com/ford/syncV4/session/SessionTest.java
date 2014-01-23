package com.ford.syncV4.session;

import android.test.AndroidTestCase;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.service.Service;

import java.util.List;

/**
 * Created by Andrew Batutin on 1/21/14.
 */
public class SessionTest extends AndroidTestCase{

    public void testSessionIsCreated() throws Exception {
        Session session = new Session();
        assertNotNull("currentSession object should be created",session);
    }

    public void testSessionIdShouldBeSet() throws Exception {
        Session session = new Session();
        session.setSessionId((byte)1);
        assertEquals("currentSession id should be 1", (byte)1, session.getSessionId());
    }

    public void testServiceAddedToServiceList() throws Exception {
        Session session = new Session();
        Service service = new Service();
        session.addService(service);
        List<Service> services = session.getServiceList();
        assertEquals("service should be at list",service, services.get(0));
    }

    public void testServiceRemovedFromServiceList() throws Exception {
        Session session = new Session();
        Service service = new Service();
        session.addService(service);
        boolean res = session.removeService(service);
        assertTrue("element should be removed", res);
        assertEquals("service list should be empty",0, session.getServiceList().size());
    }

    public void testInitialSessionCreationCreatesRPCService() throws Exception {
        Session session = Session.createSession(ServiceType.RPC, (byte) 0);
        Service service = session.getServiceList().get(0);
        assertEquals("currentSession id should be SESSION_ID", (byte) 0, session.getSessionId());
        assertEquals("should be RPC service", ServiceType.RPC, service.getServiceType());
        assertEquals("service should belong to the currentSession", session, service.getSession());
    }

    public void testRemoveServiceRemovesService() throws Exception {
        Session session = Session.createSession(ServiceType.RPC, (byte) 0);
        Service service = new Service();
        service.setSession(session);
        service.setServiceType(ServiceType.RPC);
        assertTrue("service should be removed", session.removeService(service));
    }

    public void testStopSessionClearsServiceList() throws Exception {
        Session session = Session.createSession(ServiceType.RPC, (byte) 10);
        session.stopSession();
        assertEquals("service list should be 0",  0, session.getServiceList().size());
        assertEquals("session id should be 0",0, session.getSessionId());
    }
}
