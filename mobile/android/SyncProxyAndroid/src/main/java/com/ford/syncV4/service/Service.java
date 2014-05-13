package com.ford.syncV4.service;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;

/**
 * Created by Andrew Batutin on 1/21/14.
 */
public class Service {

    // TODO: Reconsider this field, probably, as alternative, create 'sessionId' field here
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
                "syncSession=" + session +
                ", serviceType=" + serviceType +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Service service = (Service) o;

        if (serviceType != null ? !serviceType.equals(service.serviceType) : service.serviceType != null)
            return false;
        if (session != null ? !session.equals(service.session) : service.session != null)
            return false;

        return true;
    }

    @Override
    public int hashCode() {
        int result = session != null ? session.hashCode() : 0;
        result = 31 * result + (serviceType != null ? serviceType.hashCode() : 0);
        return result;
    }
}
