package com.ford.syncV4.session;

import android.test.AndroidTestCase;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.service.Service;

import java.util.List;

/**
 * Created by Andrew Batutin on 1/21/14
 */
public class SessionTest extends AndroidTestCase {

    public static final String APP_ID = "APP_ID";
    public static final String APP_ID_DEFAULT = "";
    public static final byte SESSION_ID = 0x01;

    public void testSessionIsCreated() throws Exception {
        Session session = new Session();
        assertNotNull("syncSession object should be created",session);
    }

    public void testSessionIdShouldBeSet() throws Exception {
        Session session = new Session();
        session.setSessionId(SESSION_ID);
        assertEquals("syncSession id should be 1", SESSION_ID, session.getSessionId());
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
        assertEquals("service list should be empty", 0, session.getServicesList().size());
    }

    public void testSessionAddRPCService() throws Exception {
        Session session = SessionTest.getInitializedSession();
        Service service = new Service();
        service.setServiceType(ServiceType.RPC);
        service.setSessionId(SessionTest.SESSION_ID);
        session.addService(service);

        Service rpcService = session.getServiceBySessionId(SessionTest.SESSION_ID, ServiceType.RPC);
        assertNotNull(rpcService);
        assertEquals(SessionTest.SESSION_ID, rpcService.getSessionId());
        assertEquals(ServiceType.RPC, rpcService.getServiceType());
    }

    public void testRemoveServiceRemovesService() throws Exception {
        Session session = SessionTest.getInitializedSession();
        Service service = new Service();
        service.setSessionId(SessionTest.SESSION_ID);
        service.setServiceType(ServiceType.RPC);
        session.addService(service);
        assertTrue("service should be removed", session.removeService(service));
    }

    public void testStopSessionClearsServiceList() throws Exception {
        Session session = getInitializedSession();
        session.stopSession(APP_ID);
        assertEquals("service list should be 0", 0, session.getServicesList().size());
        assertEquals("session id should be 0", 0, session.getSessionId());
    }

    public void testEmptyServicesList() {
        Session session = getInitializedSession();
        assertTrue(session.isServicesEmpty());
    }

    public void testEmptyServicesListWithNonEmptyList() {
        Session session = getInitializedSession();
        session.addService(session.createService(ServiceType.Audio_Service));
        assertFalse(session.isServicesEmpty());
    }

    public void testHasServiceByCorrectType() {
        Session session = getInitializedSession();
        Service service = session.createService(ServiceType.Audio_Service);
        service.setSessionId(SessionTest.SESSION_ID);
        session.addService(service);
        assertTrue(session.hasService(APP_ID, ServiceType.Audio_Service));
    }

    public void testHasServiceByIncorrectType() {
        Session session = getInitializedSession();
        session.createService(ServiceType.Audio_Service);
        assertFalse(session.hasService(APP_ID, ServiceType.Mobile_Nav));
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

    public static Session getInitializedSession() {
        Session session = new Session();
        session.putDefaultSessionIdToAppId(APP_ID);
        session.updateSessionId(SESSION_ID);
        return session;
    }
}