package com.ford.syncV4.service;

import com.ford.syncV4.protocol.enums.ServiceType;

/**
 * Created by Andrew Batutin on 1/21/14.
 */
public class Service {

    private byte sessionID;
    private ServiceType serviceType;

    public void setSessionID(byte sessionID) {
        this.sessionID = sessionID;
    }

    public byte getSessionID() {
        return sessionID;
    }

    public void setServiceType(ServiceType serviceType) {
        this.serviceType = serviceType;
    }

    public ServiceType getServiceType() {
        return serviceType;
    }
}
