package com.ford.syncV4.session;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.service.Service;
import com.ford.syncV4.util.logger.Logger;

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;
import java.util.Set;

/**
 * Created by Andrew Batutin on 1/21/14
 * Co-author: Yuriy Chernyshov
 */
public class Session {

    public static final byte DEFAULT_SESSION_ID = 0;
    public static final byte UNDEFINED_SESSION_ID = Byte.MIN_VALUE;

    private static final String CLASS_NAME = Session.class.getSimpleName();

    /**
     * Collection of the Session Id's mapped to App Id
     */
    private final Hashtable<String, Byte> sessionIds = new Hashtable<String, Byte>();

    private byte sessionId = DEFAULT_SESSION_ID;
    private final List<Service> servicesList = new ArrayList<Service>();

    /**
     * Assign Session Id to the first available Session
     * @param value session id
     * @return AppId associated with the assigned Session
     */
    public String updateSessionId(byte value) {
        for (String key: sessionIds.keySet()) {
            if (sessionIds.get(key) == DEFAULT_SESSION_ID) {
                sessionIds.put(key, value);
                Logger.d("Assign sessionId:" + value + " to AppId:" + key);
                return key;
            }
        }
        Logger.w("No Session Id assignments has been done!, sessionId:" + value);
        return "";
    }

    /**
     * Add new Session Id associated with a provided AppId
     * @param appId AppId
     */
    public void putDefaultSessionIdToAppId(String appId) {
        Logger.d("Add sessionId:" + DEFAULT_SESSION_ID + " to AppId:" + appId);
        sessionIds.put(appId, DEFAULT_SESSION_ID);
    }

    /**
     * Get Session Id associated with AppId
     * @param appId
     * @return
     */
    public byte getSessionIdByAppId(String appId) {
        if (sessionIds.containsKey(appId)) {
            return sessionIds.get(appId);
        }
        return DEFAULT_SESSION_ID;
    }

    public Set<String> getSessionIdsKeys() {
        return sessionIds.keySet();
    }

    public String getAppIdBySessionId(byte sessionId) {
        for (String key : sessionIds.keySet()) {
            if (key == null) {
                continue;
            }
            if (sessionIds.get(key) == null) {
                continue;
            }
            if (sessionIds.get(key).equals(sessionId)) {
                return key;
            }
        }
        return "";
    }

    /**
     * @return Returns the number of Session Id's
     */
    public int getSessionIdsNumber() {
        return sessionIds.size();
    }

    @Deprecated
    public void setSessionId(byte sessionId) {
        this.sessionId = sessionId;
    }

    @Deprecated
    public byte getSessionId() {
        return sessionId;
    }

    public boolean hasSessionId(byte sessionId) {
        return sessionIds.contains(sessionId);
    }

    public void addService(Service service) {
        Logger.i(CLASS_NAME + " Add " + service + ", contains:" + servicesList.contains(service));
        if (servicesList.contains(service)) {
            return;
        }
        servicesList.add(service);
    }

    public List<Service> getServicesList() {
        return servicesList;
    }

    public Service getServiceBySessionId(byte sessionId, ServiceType serviceType) {
        for (Service service : servicesList) {
            if (service.getSessionId() == sessionId && service.getServiceType() == serviceType) {
                Logger.i(CLASS_NAME + " GetService '" + serviceType + " ' BySessionId:" + sessionId);
                return service;
            }
        }
        Logger.w(CLASS_NAME + " getServiceBySessionId return null");
        return null;
    }

    public boolean hasService(String appId, ServiceType serviceType) {
        byte sessionId = getSessionIdByAppId(appId);
        return hasService(sessionId, serviceType);
    }

    public boolean hasService(byte sessionId, ServiceType serviceType) {
        if (servicesList.isEmpty()) {
            return false;
        }
        for (Service service : servicesList) {
            if (service.getSessionId() == sessionId && service.getServiceType() == serviceType) {
                return true;
            }
        }
        return false;
    }

    public int getServicesNumber() {
        return servicesList.size();
    }

    public boolean isServicesEmpty() {
        return servicesList.isEmpty();
    }

    @Override
    public String toString() {
        return "Session{" +
                "sessionId=" + sessionId + ", servicesList size=" + servicesList.size() + '}';
    }

    public boolean removeService(Service service) {
        boolean result = servicesList.remove(service);
        Logger.i(CLASS_NAME + " Remove " + service.getServiceType() + ", complete:" + result);
        return result;
    }

    public Service createService(ServiceType serviceType) {
        Logger.i(CLASS_NAME + " Create " + serviceType);
        Service service = new Service();
        service.setServiceType(serviceType);
        return service;
    }

    public void stopSession(String appId) {
        byte sessionId = getSessionIdByAppId(appId);
        List<Service> tobeRemoved = new ArrayList<Service>();
        for (Service service : servicesList) {
            Logger.i(CLASS_NAME + " Stop session sesId:" + service.getSessionId() + " " + sessionId);
            if (service.getSessionId() == sessionId) {
                tobeRemoved.add(service);
            }
        }
        for (Service service: tobeRemoved) {
            Logger.i(CLASS_NAME + " Stop session remove:" + service);
            servicesList.remove(service);
        }
        tobeRemoved.clear();
        sessionIds.remove(appId);

        Logger.i(CLASS_NAME + " Stop appId:" + appId);
    }
}