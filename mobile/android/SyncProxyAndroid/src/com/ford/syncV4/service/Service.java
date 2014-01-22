package com.ford.syncV4.service;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;

/**
 * Created by Andrew Batutin on 1/21/14.
 */
public class Service {

    private Session session;
    private ServiceType serviceType;

    public void setSession(Session session) {
        this.session = session;
    }

    public Session getSession() {
        return session;
    }

    public void setServiceType(ServiceType serviceType) {
        this.serviceType = serviceType;
    }

    public ServiceType getServiceType() {
        return serviceType;
    }

    @Override
    public String toString() {
        return "Service{" +
                "currentSession=" + session +
                ", serviceType=" + serviceType +
                '}';
    }

    public static Service createService(ServiceType mobile_nav, byte sessionID) {
        Service service = new Service();
        return null;
    }
}
