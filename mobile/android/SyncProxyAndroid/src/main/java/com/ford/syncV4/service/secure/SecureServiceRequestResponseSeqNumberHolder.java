package com.ford.syncV4.service.secure;

import com.ford.syncV4.protocol.enums.ServiceType;

import java.util.Hashtable;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 12.03.14
 * Time: 15:16
 */

/**
 * TEMPORARY helper class to manage request / response seq numbering associated with the service
 * type
 */
public class SecureServiceRequestResponseSeqNumberHolder {

    private static volatile SecureServiceRequestResponseSeqNumberHolder sInstance = null;
    private Hashtable<Integer, String> mHashtable = new Hashtable<Integer, String>();
    private int mSeqNumberInitValue = 12345;

    private SecureServiceRequestResponseSeqNumberHolder() {
        sInstance = this;
    }

    /**
     * Double-checked singleton fetching
     * @return
     */
    public static SecureServiceRequestResponseSeqNumberHolder getInstance() {
        if (sInstance == null) {
            synchronized(SecureServiceRequestResponseSeqNumberHolder.class) {
                if (sInstance == null) {
                    new SecureServiceRequestResponseSeqNumberHolder();
                }
            }
        }
        return sInstance;
    }

    /**
     * @return next seq number value
     */
    public int getNextSeqNumber() {
        return mSeqNumberInitValue++;
    }

    /**
     * Associate sequence number of the request with a Service Type
     *
     * @param seqNumber   sequence number
     * @param serviceName service type name
     */
    public void addServiceWithKey(int seqNumber, String serviceName) {
        mHashtable.put(seqNumber, serviceName);
    }

    /**
     * @param seqNumber sequence number
     * @return sequence number of the request with a Service Type
     */
    public ServiceType getServiceTypeBySeqNumber(int seqNumber) {
        String name = mHashtable.get(seqNumber);
        if (name == null) {
            return null;
        }
        if (name.isEmpty()) {
            return null;
        }
        if (name.equals(ServiceType.HEARTBEAT_SERVICE_NAME)) {
            return ServiceType.Heartbeat;
        }
        if (name.equals(ServiceType.RPC_NAME)) {
            return ServiceType.RPC;
        }
        if (name.equals(ServiceType.BULK_DATA_NAME)) {
            return ServiceType.Bulk_Data;
        }
        if (name.equals(ServiceType.MOBILE_NAV_NAME)) {
            return ServiceType.Mobile_Nav;
        }
        if (name.equals(ServiceType.AUDIO_SERVICE_NAME)) {
            return ServiceType.Audio_Service;
        }
        if (name.equals(ServiceType.SECURE_SERVICE_NAME)) {
            return ServiceType.Secure_Service;
        }
        return null;
    }
}