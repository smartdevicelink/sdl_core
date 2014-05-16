package com.ford.syncV4.service;

import com.ford.syncV4.protocol.enums.ServiceType;

/**
 * Created by Andrew Batutin on 1/21/14
 * Co-author Chernyshov Yuriy
 */
public class Service {

    private byte sessionId;
    private ServiceType serviceType;

    public void setSessionId(byte value) {
        sessionId = value;
    }

    public byte getSessionId() {
        return sessionId;
    }

    public void setServiceType(ServiceType value) {
        serviceType = value;
    }

    public ServiceType getServiceType() {
        return serviceType;
    }

    @Override
    public String toString() {
        return "Service {sessionId:" + sessionId + ", serviceType:" + serviceType + "}";
    }

    /*@Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Service service = (Service) o;

        if (serviceType != null ? !serviceType.equals(service.serviceType) : service.serviceType != null)
            return false;
        if (sessionId != null ? !sessionId.equals(service.sessionId) : service.sessionId != null)
            return false;

        return true;
    }

    @Override
    public int hashCode() {
        int result = sessionId != null ? sessionId.hashCode() : 0;
        result = 31 * result + (serviceType != null ? serviceType.hashCode() : 0);
        return result;
    }*/
}
