package com.ford.syncV4.service;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;

/**
 * Created by Andrew Batutin on 1/21/14
 * Co-author Chernyshov Yuriy
 */
public class Service {

    private static final String CLASS_NAME = Service.class.getSimpleName();

    private byte sessionId = Session.UNDEFINED_SESSION_ID;
    /**
     * By default a Service is RPC type
     */
    private ServiceType serviceType = ServiceType.RPC;

    public void setSessionId(byte value) {
        sessionId = value;
    }

    public byte getSessionId() {
        return sessionId;
    }

    public void setServiceType(ServiceType value) {
        if (value == null) {
            throw new NullPointerException(CLASS_NAME + " set Service can not be null");
        }
        serviceType = value;
    }

    public ServiceType getServiceType() {
        return serviceType;
    }

    @Override
    public String toString() {
        return "Service {sessionId:" + sessionId + ", serviceType:" + serviceType + "}";
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        if (o instanceof Service) {
            Service service = (Service) o;
            if (sessionId == service.sessionId && serviceType.equals(service.serviceType))
                return true;
        }

        return false;
    }

    /*@Override
    public int hashCode() {
        int result = sessionId != null ? sessionId.hashCode() : 0;
        result = 31 * result + (serviceType != null ? serviceType.hashCode() : 0);
        return result;
    }*/
}