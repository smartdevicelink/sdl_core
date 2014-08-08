package com.ford.syncV4.service;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.APIConstants;

/**
 * Created by Andrew Batutin on 1/21/14
 * Co-author Chernyshov Yuriy
 */
public class Service {

    private boolean encrypted;
    private static final String CLASS_NAME = Service.class.getSimpleName();

    private String appId = APIConstants.APP_ID_EMPTY;
    /**
     * By default a Service is RPC type
     */
    private ServiceType serviceType = ServiceType.RPC;

    public void setAppId(String value) {
        if (value == null) {
            return;
        }
        appId = value;
    }

    public String getAppId() {
        return appId;
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
        return "Service{serviceType=" + serviceType + ", encrypted=" + encrypted + '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        if (o instanceof Service) {
            Service service = (Service) o;
            if (appId.equals(service.appId) && serviceType.equals(service.serviceType))
                return true;
        }

        return false;
    }

    public void setEncrypted(boolean encrypted) {
        this.encrypted = encrypted;
    }

    public boolean isEncrypted() {
        return encrypted;
    }
}
