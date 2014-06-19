package com.ford.syncV4.session;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.APIConstants;
import com.ford.syncV4.service.Service;
import com.ford.syncV4.util.logger.Logger;

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Created by Andrew Batutin on 1/21/14
 * Co-author: Yuriy Chernyshov
 */
public class Session {

    /**
     * Default Id of the Session
     */
    public static final byte DEFAULT_SESSION_ID = 0;
    /**
     * Undefined value of the Session Id, for example in case of the initialization of the Service
     */
    public static final byte UNDEFINED_SESSION_ID = Byte.MIN_VALUE;

    private static final String CLASS_NAME = Session.class.getSimpleName();

    /**
     * Collection of the Session Id's mapped to App Id
     */
    private final ConcurrentHashMap<String, Byte> sessionIds = new ConcurrentHashMap<String, Byte>();

    /**
     * Collection of the non Sync Services (RPC, Navi, Audio, etc ...)
     */
    private final List<Service> servicesList = new ArrayList<Service>();

    /**
     * Assign Session Id to the first available {@link com.ford.syncV4.session.Session} (assuming
     * that available Session has {@code DEFAULT_SESSION_ID}). This can be done for example in case
     * of Start Service ACK for the RPC Service which is received with a Session Id value.
     *
     * @param value Session Id
     *
     * @return AppId associated with the assigned {@link com.ford.syncV4.session.Session}
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
     * Add new AppId in to the collection and assign {@code DEFAULT_SESSION_ID} to it. Associated
     * Session Id will be updated later on when Start Service ACK will be received.
     *
     * @param appId AppId
     */
    public void addAppId(String appId) {
        Logger.d("Add sessionId:" + DEFAULT_SESSION_ID + " to AppId:" + appId);
        sessionIds.put(appId, DEFAULT_SESSION_ID);
    }

    /**
     * Get Session Id associated with provided AppId.
     *
     * @param appId Application Id
     *
     * @return Session Id
     */
    public byte getSessionIdByAppId(String appId) {
        if (sessionIds.containsKey(appId)) {
            return sessionIds.get(appId);
        }
        return DEFAULT_SESSION_ID;
    }

    /**
     * Get AppId associated with provided Session Id.
     *
     * @param sessionId Session Id
     *
     * @return Application Id value
     */
    public String getAppIdBySessionId(byte sessionId) {
        for (String key : sessionIds.keySet()) {
            if (key == null) {
                continue;
            }
            if (sessionIds.get(key) == null) {
                continue;
            }
            if (sessionIds.get(key) == sessionId) {
                return key;
            }
        }
        return "";
    }

    /**
     * @return a Set of the Session Id's
     */
    public Set<String> getSessionIdsKeys() {
        return sessionIds.keySet();
    }

    /**
     * @return a number of Session Id's
     */
    public int getSessionIdsNumber() {
        return sessionIds.size();
    }

    /**
     * Check whether Session with provided Id exists int he collection
     *
     * @param sessionId Session Id
     *
     * @return <b>true</b> if exists, <b>false</b> otherwise
     */
    public boolean hasSessionId(byte sessionId) {
        return sessionIds.contains(sessionId);
    }

    /**
     * Add {@link com.ford.syncV4.service.Service} tot he collection
     *
     * @param service {@link com.ford.syncV4.service.Service}
     */
    public void addService(Service service) {
        Logger.i(CLASS_NAME + " Add " + service + ", contains:" + servicesList.contains(service));
        if (servicesList.contains(service)) {
            Logger.d(CLASS_NAME + " Service already exists");
            return;
        }
        servicesList.add(service);
    }

    /**
     * Get {@link com.ford.syncV4.service.Service} by provided Session Id
     *
     * @param appId   Session Id
     * @param serviceType {@link com.ford.syncV4.protocol.enums.ServiceType}
     *
     * @return {@link com.ford.syncV4.service.Service}
     */
    public Service getServiceByAppId(String appId, ServiceType serviceType) {
        for (Service service : servicesList) {
            if (service.getAppId().equals(appId) && service.getServiceType() == serviceType) {
                Logger.i(CLASS_NAME + " GetService '" + serviceType + " ' BySessionId:" + appId);
                return service;
            }
        }
        Logger.w(CLASS_NAME + " getServiceByAppId return null");
        return null;
    }

    /**
     * Check whether provided {@link com.ford.syncV4.service.Service} exists in the collection
     *
     * @param service {@link com.ford.syncV4.service.Service}
     *
     * @return true if {@link com.ford.syncV4.service.Service} exists, false - otherwise
     */
    public boolean hasService(Service service) {
        return service != null && servicesList.contains(service);
    }

    /**
     * Check whether {@link com.ford.syncV4.service.Service} with provided
     * {@link com.ford.syncV4.protocol.enums.ServiceType} and Session Id exists in the collection
     *
     * @param appId       Application Id
     * @param serviceType {@link com.ford.syncV4.protocol.enums.ServiceType}
     *
     * @return true if {@link com.ford.syncV4.service.Service} exists, false - otherwise
     */
    public boolean hasService(String appId, ServiceType serviceType) {
        if (servicesList.isEmpty()) {
            return false;
        }
        for (Service service : servicesList) {
            if (service.getAppId().equals(appId) && service.getServiceType() == serviceType) {
                return true;
            }
        }
        return false;
    }

    /**
     * @return number of the services in the collection
     */
    public int getServicesNumber() {
        return servicesList.size();
    }

    /**
     * @return whether collection is empty or not
     */
    public boolean isServicesEmpty() {
        return servicesList.isEmpty();
    }

    /**
     * Removes a {@link com.ford.syncV4.service.Service} from collection by provided Session Id
     * and {@link com.ford.syncV4.protocol.enums.ServiceType}
     *
     * @param appId       Application Id
     * @param serviceType {@link com.ford.syncV4.protocol.enums.ServiceType}
     *
     * @return true in case of success, false - otherwise
     */
    public boolean removeService(String appId, ServiceType serviceType) {
        for (Service aService : servicesList) {
            if ((aService.getAppId().equals(appId)) &&
                    (aService.getServiceType() == serviceType)) {
                return removeService(aService);
            }
        }
        return false;
    }

    /**
     * Removes a {@link com.ford.syncV4.service.Service} from collection
     *
     * @param service {@link com.ford.syncV4.service.Service}
     *
     * @return true in case of success, false - otherwise
     */
    public boolean removeService(Service service) {
        boolean result = servicesList.remove(service);
        Logger.i(CLASS_NAME + " Remove " + service.getServiceType() + ", complete:" + result);
        return result;
    }

    /**
     * Creates a {@link com.ford.syncV4.service.Service} by
     * {@link com.ford.syncV4.protocol.enums.ServiceType}
     *
     * @param serviceType {@link com.ford.syncV4.protocol.enums.ServiceType}
     *
     * @return created {@link com.ford.syncV4.service.Service}
     */
    public Service createService(String appId, ServiceType serviceType) {
        Logger.i(CLASS_NAME + " Create " + serviceType + " for appId:" + appId);
        Service service = new Service();
        service.setServiceType(serviceType);
        service.setAppId(appId);
        return service;
    }

    /**
     * Stops a {@link com.ford.syncV4.session.Session} associated with provided Application Id.
     * Remove all associated {@link com.ford.syncV4.service.Service}'s and drop Session Id to
     * {@code DEFAULT_SESSION_ID}
     *
     * @param appId Application Id
     */
    public void stopSession(String appId) {
        List<Service> tobeRemoved = new ArrayList<Service>();
        for (Service service : servicesList) {
            //Logger.i(CLASS_NAME + " Stop session appId:" + service.getAppId() + " - " + appId);
            if (service.getAppId().equals(appId)) {
                tobeRemoved.add(service);
            }
        }
        for (Service service: tobeRemoved) {
            boolean result = removeService(service);
            Logger.i(CLASS_NAME + " Stop session remove:" + service + " result:" + result);
        }
        tobeRemoved.clear();
        int result = sessionIds.remove(appId);

        Logger.i(CLASS_NAME + " Stop appId:" + appId + " res:" + result);
    }

    /**
     * Invalidates provided Application Id, clear all Services associated and remove it from the list
     *
     * @param appId Application Id
     *
     * @return true in case of success, false - otherwise
     */
    public boolean invalidateAppId(String appId) {

        // TODO : Implement

        return true;
    }

    @Override
    public String toString() {
        return "Session{" +
                "sessionId's:" + sessionIds + ", servicesList:" + servicesList + '}';
    }

    /**
     * Will be removed soon, do not use it any more
     * @return
     */
    @Deprecated
    public List<Service> getServicePool() {
        return servicesList;
    }
}