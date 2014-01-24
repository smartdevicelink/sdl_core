package com.ford.syncV4.session;

import android.util.Log;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.service.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Andrew Batutin on 1/21/14.
 */
public class Session {

    public static final byte DEFAULT_SESSION_ID = 0;

    private static final String TAG = "Session";

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

    private byte sessionId = DEFAULT_SESSION_ID;
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
        // TODO: method 'create' must not do additional actions, such as add to collection
        serviceList.add(service);
        return service;
    }

    public void stopSession() {
        Log.i(TAG, "Stop " + toString());
        serviceList.clear();
        sessionId = 0;
    }
}
