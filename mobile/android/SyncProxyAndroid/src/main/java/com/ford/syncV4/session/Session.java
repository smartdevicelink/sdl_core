package com.ford.syncV4.session;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.service.Service;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.util.logger.Logger;

import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

/**
 * Created by Andrew Batutin on 1/21/14
 * Co-author: Yuriy Chernyshov
 */
public class Session {

    public static final byte DEFAULT_SESSION_ID = 0;

    public static Session createSession(ServiceType serviceType, byte sessionID) {
        Session session = new Session();
        session.setSessionId(sessionID);
        Service service = new Service();
        service.setSession(session);
        service.setServiceType(serviceType);
        session.addService(service);
        return session;
    }

    private static final String CLASS_NAME = Session.class.getSimpleName();

    private static final Vector<SyncConnection> shareConnections = new Vector<SyncConnection>();

    /**
     * Collection of the Session Id's
     */
    private final Vector<Byte> sessionIds = new Vector<Byte>();

    private byte sessionId = DEFAULT_SESSION_ID;
    private final List<Service> serviceList = new ArrayList<Service>();

    /**
     * Add Session Id to the Collection
     * @param value session id
     */
    public void addSessionId(byte value) {
        sessionIds.add(value);
        Logger.d(CLASS_NAME + " add sessionId:" + value);
    }

    /**
     * Get Session Id from Collection at specified index
     * @param value index of the Session Id at the Collection
     * @return Session Id, or 0 if such position does not exists
     */
    public byte getSessionIdAt(int value) {
        if (value >=0 && value < sessionIds.size()) {
            return sessionIds.get(value);
        }
        return (byte)(0x00);
    }

    /**
     * @return Returns the number of Session Id's
     */
    public int getSessionIdsNumber() {
        return sessionIds.size();
    }

    public void setSessionId(byte sessionId) {
        this.sessionId = sessionId;
    }

    public byte getSessionId() {
        return sessionId;
    }

    public void addService(Service service) {
        Logger.i(CLASS_NAME + " Add " + service + ", contains:" + serviceList.contains(service));
        if (serviceList.contains(service)) {
            return;
        }
        serviceList.add(service);
    }

    public List<Service> getServiceList() {
        return serviceList;
    }

    public boolean hasService(ServiceType serviceType) {
        if (serviceList.isEmpty()) {
            return false;
        }
        for (Service service : serviceList) {
            if (service.getServiceType() == serviceType) {
                return true;
            }
        }
        return false;
    }

    public int getServicesNumber() {
        return serviceList.size();
    }

    public boolean isServicesEmpty() {
        return serviceList.isEmpty();
    }

    @Override
    public String toString() {
        return "Session{" +
                "sessionId=" + sessionId + ", serviceList size=" + serviceList.size() + '}';
    }

    public boolean removeService(Service service) {
        boolean result = serviceList.remove(service);
        Logger.i(CLASS_NAME + " Remove " + service.getServiceType() + ", complete:" + result);
        return result;
    }

    public Service createService(ServiceType serviceType) {
        Logger.i(CLASS_NAME + " Create " + serviceType);
        Service service = new Service();
        service.setServiceType(serviceType);
        service.setSession(this);
        return service;
    }

    public void stopSession() {
        serviceList.clear();
        sessionId = 0;
        Logger.i(CLASS_NAME + " Stop " + toString());
    }
}