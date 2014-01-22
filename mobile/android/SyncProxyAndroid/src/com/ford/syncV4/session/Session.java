package com.ford.syncV4.session;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.service.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Andrew Batutin on 1/21/14.
 */
public class Session {

    public static Session createSession(ServiceType serviceType,
                                         byte sessionID) {
        Session session = new Session();
        session.setSessionId(sessionID);
        Service service = new Service();
        service.setSession(session);
        service.setServiceType(serviceType);
        session.addService(service);
        return session;
    }


    private byte sessionId;
    private List<Service> serviceList = new ArrayList<Service>();

    public void setSessionId(byte sessionId) {
        this.sessionId = sessionId;
    }

    public byte getSessionId() {
        return sessionId;
    }

    public void addService(Service service) {
        serviceList.add(service);
    }

    public List<Service> getServiceList() {
        return serviceList;
    }

    public void setServiceList(List<Service> serviceList) {
        this.serviceList = serviceList;
    }

    @Override
    public String toString() {
        return "Session{" +
                "sessionId=" + sessionId +
                ", serviceList size=" + serviceList.size() +
                '}';
    }

    public boolean removeService(Service service) {
        return serviceList.remove(service);
    }

    public Service createService(ServiceType serviceType) {
        Service service = new Service();
        service.setServiceType(serviceType);
        service.setSession(this);
        serviceList.add(service);
        return service;
    }
}
