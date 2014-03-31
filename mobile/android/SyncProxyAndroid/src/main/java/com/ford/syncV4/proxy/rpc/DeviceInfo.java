package com.ford.syncV4.proxy.rpc;

import android.util.Log;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;

import java.util.Hashtable;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 21.03.14
 * Time: 12:36
 */

/**
 * This class holds various information about connecting device.
 */
public class DeviceInfo extends RPCStruct {

    /**
     * Device Info LOG tag
     */
    private static final String LOG_TAG = DeviceInfo.class.getSimpleName();

    /**
     * Max length of the device model name
     */
    private static final int HARDWARE_MAX_LENGTH = 500;

    /**
     * Max length of the device firmware revision
     */
    private static final int FIRMWARE_REV_MAX_LENGTH = 500;

    /**
     * Max length of the device operating system name
     */
    private static final int OS_MAX_LENGTH = 500;

    /**
     * Max length of the device operating system version name
     */
    private static final int OS_VERSION_MAX_LENGTH = 500;

    /**
     * Max length of the device mobile carrier name (if applicable)
     */
    private static final int CARRIER_MAX_LENGTH = 500;

    /**
     * Max number of the device RF COMM ports
     */
    private static final int MAX_NUMBER_RF_COMM_PORTS = 100;

    /**
     * Min number of the device RF COMM ports number
     */
    private static final int MIN_NUMBER_RF_COMM_PORTS = 0;

    /**
     * Empty constructor
     */
    public DeviceInfo() {

    }

    /**
     * Constructor
     * @param hashTable hash table data structure to be parsed into fields values
     */
    public DeviceInfo(Hashtable<String, Object> hashTable) {
        super(hashTable);
    }

    /**
     * Return a device model
     * @return device model
     */
    public String getHardware() {
        final Object object = store.get(Names.hardware);
        if (object instanceof String) {
            String value = (String) object;
            if (value.length() > HARDWARE_MAX_LENGTH) {
                throw new IllegalArgumentException(DeviceInfo.class.getSimpleName() +
                        " getHardware, value length can not be greater then " +
                        HARDWARE_MAX_LENGTH + ". Provided value length:" +
                        value.length());
            }
            return value;
        }
        Log.w(LOG_TAG, " getHardware is not a String, actual value:" + object);
        return null;
    }

    /**
     * Set a device model
     * @param hardware device model
     */
    public void setHardware(String hardware) {
        if (hardware != null) {
            if (hardware.length() > HARDWARE_MAX_LENGTH) {
                throw new IllegalArgumentException(DeviceInfo.class.getSimpleName() +
                        " setHardware, value length can not be greater then " +
                        HARDWARE_MAX_LENGTH + ". Provided value length:" +
                        hardware.length());
            }
            store.put(Names.hardware, hardware);
        } else {
            store.remove(Names.hardware);
        }
    }

    /**
     * Return a device firmware revision
     * @return device firmware revision
     */
    public String getFirmwareRev() {
        final Object object = store.get(Names.firmwareRev);
        if (object instanceof String) {
            String value = (String) object;
            if (value.length() > FIRMWARE_REV_MAX_LENGTH) {
                throw new IllegalArgumentException(DeviceInfo.class.getSimpleName() +
                        " getFirmwareRev, value length can not be greater then " +
                        FIRMWARE_REV_MAX_LENGTH + ". Provided value length:" +
                        value.length());
            }
            return value;
        }
        Log.w(LOG_TAG, " getFirmwareRev is not a String, actual value:" + object);
        return null;
    }

    /**
     * Set a device firmware revision
     * @param firmwareRev device firmware revision
     */
    public void setFirmwareRev(String firmwareRev) {
        if (firmwareRev != null) {
            if (firmwareRev.length() > FIRMWARE_REV_MAX_LENGTH) {
                throw new IllegalArgumentException(DeviceInfo.class.getSimpleName() +
                        " setFirmwareRev, value length can not be greater then " +
                        FIRMWARE_REV_MAX_LENGTH + ". Provided value length:" +
                        firmwareRev.length());
            }
            store.put(Names.firmwareRev, firmwareRev);
        } else {
            store.remove(Names.firmwareRev);
        }
    }

    /**
     * Return a device operating system
     * @return device operating system
     */
    public String getOS() {
        final Object object = store.get(Names.os);
        if (object instanceof String) {
            String value = (String) object;
            if (value.length() > OS_MAX_LENGTH) {
                throw new IllegalArgumentException(DeviceInfo.class.getSimpleName() +
                        " getOS, value length can not be greater then " +
                        OS_VERSION_MAX_LENGTH + ". Provided value length:" +
                        value.length());
            }
            return value;
        }
        Log.w(LOG_TAG, " getOS is not a String, actual value:" + object);
        return null;
    }

    /**
     * Set a device operating system
     * @param deviceOS device operating system
     */
    public void setOS(String deviceOS) {
        if (deviceOS != null) {
            if (deviceOS.length() > OS_MAX_LENGTH) {
                throw new IllegalArgumentException(DeviceInfo.class.getSimpleName() +
                        " setOS, value length can not be greater then " +
                        OS_MAX_LENGTH + ". Provided value length:" +
                        deviceOS.length());
            }
            store.put(Names.os, deviceOS);
        } else {
            store.remove(Names.os);
        }
    }

    /**
     * Return a device operating system version
     * @return device operating system version
     */
    public String getOSVersion() {
        final Object object = store.get(Names.osVersion);
        if (object instanceof String) {
            String value = (String) object;
            if (value.length() > OS_VERSION_MAX_LENGTH) {
                throw new IllegalArgumentException(DeviceInfo.class.getSimpleName() +
                        " getOSVersion, value length can not be greater then " +
                        OS_VERSION_MAX_LENGTH + ". Provided value length:" +
                        value.length());
            }
            return value;
        }
        Log.w(LOG_TAG, " getOSVersion is not a String, actual value:" + object);
        return null;
    }

    /**
     * Set a device operating system version
     * @param deviceOSVersion device operating system version
     */
    public void setOSVersion(String deviceOSVersion) {
        if (deviceOSVersion != null) {
            if (deviceOSVersion.length() > OS_VERSION_MAX_LENGTH) {
                throw new IllegalArgumentException(DeviceInfo.class.getSimpleName() +
                        " setOSVersion, value length can not be greater then " +
                        OS_VERSION_MAX_LENGTH + ". Provided value length:" +
                        deviceOSVersion.length());
            }
            store.put(Names.osVersion, deviceOSVersion);
        } else {
            store.remove(Names.osVersion);
        }
    }

    /**
     * Return a device mobile carrier (if applicable)
     * @return device mobile carrier (if applicable)
     */
    public String getCarrier() {
        final Object object = store.get(Names.carrier);
        if (object instanceof String) {
            String value = (String) object;
            if (value.length() > CARRIER_MAX_LENGTH) {
                throw new IllegalArgumentException(DeviceInfo.class.getSimpleName() +
                        " getCarrier, value length can not be greater then " +
                        CARRIER_MAX_LENGTH + ". Provided value length:" +
                        value.length());
            }
            return value;
        }
        Log.w(LOG_TAG, " getCarrier is not a String, actual value:" + object);
        return null;
    }

    /**
     * Set a device mobile carrier (if applicable)
     * @param carrier device mobile carrier (if applicable)
     */
    public void setCarrier(String carrier) {
        if (carrier != null) {
            if (carrier.length() > CARRIER_MAX_LENGTH) {
                throw new IllegalArgumentException(DeviceInfo.class.getSimpleName() +
                        " setCarrier, value length can not be greater then " +
                        CARRIER_MAX_LENGTH + ". Provided value length:" +
                        carrier.length());
            }
            store.put(Names.carrier, carrier);
        } else {
            store.remove(Names.carrier);
        }
    }

    /**
     * Return max number of the device RF COM ports.<br/>Omitted if connected not via BT
     * @return max number of the device RF COM ports
     */
    public Integer getMaxNumberRFCOMMPorts() {
        final Object object = store.get(Names.maxNumberRFCOMMPorts);
        if (object instanceof Integer) {
            Integer value = (Integer) object;
            if (value < MIN_NUMBER_RF_COMM_PORTS) {
                Log.w(LOG_TAG, "GetMaxNumberRFCOMMPorts value less then " + MIN_NUMBER_RF_COMM_PORTS +
                        ", return " + MIN_NUMBER_RF_COMM_PORTS);
                return MIN_NUMBER_RF_COMM_PORTS;
            }
            if (value > MAX_NUMBER_RF_COMM_PORTS) {
                Log.w(LOG_TAG, "GetMaxNumberRFCOMMPorts value greater then " + MAX_NUMBER_RF_COMM_PORTS +
                        ", return " + MAX_NUMBER_RF_COMM_PORTS);
                return MAX_NUMBER_RF_COMM_PORTS;
            }
            return value;
        }
        Log.w(LOG_TAG, " getMaxNumberRFCOMMPorts is not a Integer, actual value:" + object);
        return null;
    }

    /**
     * Set max number of the device RF COM ports.<br/>Omitted if connected not via BT
     * @param maxNumberRFCOMMPorts max number of the device RF COM ports
     */
    public void setMaxNumberRFCOMMPorts(Integer maxNumberRFCOMMPorts) {
        if (maxNumberRFCOMMPorts != null) {
            if (maxNumberRFCOMMPorts < MIN_NUMBER_RF_COMM_PORTS) {
                throw new IllegalArgumentException(DeviceInfo.class.getSimpleName() +
                        " setMaxNumberRFCOMMPorts, value can not be less then " +
                        MIN_NUMBER_RF_COMM_PORTS + ". Provided value:" +
                        maxNumberRFCOMMPorts);
            }
            if (maxNumberRFCOMMPorts > MAX_NUMBER_RF_COMM_PORTS) {
                throw new IllegalArgumentException(DeviceInfo.class.getSimpleName() +
                        " setMaxNumberRFCOMMPorts, value can not be greater then " +
                        MAX_NUMBER_RF_COMM_PORTS + ". Provided value:" +
                        maxNumberRFCOMMPorts);
            }
            store.put(Names.maxNumberRFCOMMPorts, maxNumberRFCOMMPorts);
        } else {
            store.remove(Names.maxNumberRFCOMMPorts);
        }
    }
}