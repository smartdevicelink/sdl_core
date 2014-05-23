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
        assertNotNull("syncSession object should be created",session);
    }

    public void testSessionIdShouldBeSet() throws Exception {
        Session session = new Session();
        session.setSessionId((byte)1);
        assertEquals("syncSession id should be 1", (byte)1, session.getSessionId());
    }

    public void testServiceAddedToServiceList() throws Exception {
        Session session = new Session();
        Service service = new Service();
        session.addService(service);
        List<Service> services = session.getServicesList();
        assertEquals("service should be at list",service, services.get(0));
    }

    public void testServiceRemovedFromServiceList() throws Exception {
        Session session = new Session();
        Service service = new Service();
        session.addService(service);
        boolean res = session.removeService(service);
        assertTrue("element should be removed", res);
        assertEquals("service list should be empty",0, session.getServicesList().size());
    }

    public void testInitialSessionCreationCreatesRPCService() throws Exception {
        Session session = Session.createSession(ServiceType.RPC, (byte) 0);
        Service service = session.getServicesList().get(0);
        assertEquals("syncSession id should be SESSION_ID", (byte) 0, session.getSessionId());
        assertEquals("should be RPC service", ServiceType.RPC, service.getServiceType());
        assertEquals("service should belong to the syncSession", session, service.getSessionId());
    }

    public void testRemoveServiceRemovesService() throws Exception {
        Session session = Session.createSession(ServiceType.RPC, (byte) 0);
        Service service = new Service();
        service.setSessionId(Session.DEFAULT_SESSION_ID);
        service.setServiceType(ServiceType.RPC);
        assertTrue("service should be removed", session.removeService(service));
    }

    public void testStopSessionClearsServiceList() throws Exception {
        Session session = Session.createSession(ServiceType.RPC, (byte) 10);
        session.stopSession();
        assertEquals("service list should be 0",  0, session.getServicesList().size());
        assertEquals("session id should be 0",0, session.getSessionId());
    }

    public void testEmptyServicesList() {
        Session session = new Session();
        assertTrue(session.isServicesEmpty());
    }

    public void testEmptyServicesListWithNonEmptyList() {
        Session session = new Session();
        session.addService(session.createService(ServiceType.Audio_Service));
        assertFalse(session.isServicesEmpty());
    }

    public void testHasServiceByCorrectType() {
        Session session = new Session();
        Service service = session.createService(ServiceType.Audio_Service);
        session.addService(service);
        assertTrue(session.hasService(ServiceType.Audio_Service));
    }

    public void testHasServiceByIncorrectType() {
        Session session = new Session();
        session.createService(ServiceType.Audio_Service);
        assertFalse(session.hasService(ServiceType.Mobile_Nav));
    }

    public void testPreventAddServiceWithSameType() {
        Session session = new Session();
        Service service_A = session.createService(ServiceType.Audio_Service);
        Service service_B = session.createService(ServiceType.Audio_Service);
        Service service_C = session.createService(ServiceType.Audio_Service);
        session.addService(service_A);
        session.addService(service_B);
        session.addService(service_C);
        assertEquals(1, session.getServicesNumber());
    }
}