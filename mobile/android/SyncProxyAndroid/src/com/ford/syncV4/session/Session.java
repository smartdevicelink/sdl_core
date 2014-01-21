package com.ford.syncV4.session;

import com.ford.syncV4.service.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Andrew Batutin on 1/21/14.
 */
public class Session {
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
                ", serviceList=" + serviceList +
                '}';
    }

    public boolean removeService(Service service) {
        return serviceList.remove(service);
    }
}
