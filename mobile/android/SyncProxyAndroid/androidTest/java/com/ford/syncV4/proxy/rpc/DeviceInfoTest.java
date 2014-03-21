package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.constants.Names;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Random;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 21.03.14
 * Time: 13:29
 */
public class DeviceInfoTest extends TestCase {

    private static final String HARDWARE_LEGAL = "Android Galaxy Nexus";
    private static final String FIRMWARE_REV_LEGAL = "API level 18";
    private static final String DEVICE_OS_LEGAL = "Android";
    private static final String DEVICE_OS_VERSION_LEGAL = "JELLY_BEAN_MR2";
    private static final String CARRIER_LEGAL = "Kievstar";
    private static final Integer MAX_RF_COMM_PORTS_NUMBER_LEGAL = 2;
    private static final int MAX_STRING_LENGTH = 500;
    private static final int MAX_INT_LENGTH = 100;

    public void testSerialization() throws JSONException {
        DeviceInfo deviceInfo = getDeviceInfo();

        int fieldsNumber = 6;

        JSONObject jsonObject = deviceInfo.serializeJSON();
        assertEquals(fieldsNumber, jsonObject.length());
        assertEquals(HARDWARE_LEGAL, jsonObject.getString(Names.hardware));
        assertEquals(FIRMWARE_REV_LEGAL, jsonObject.getString(Names.firmwareRev));
        assertEquals(DEVICE_OS_LEGAL, jsonObject.getString(Names.os));
        assertEquals(DEVICE_OS_VERSION_LEGAL, jsonObject.getString(Names.osVersion));
        assertEquals(CARRIER_LEGAL, jsonObject.getString(Names.carrier));
        assertEquals(MAX_RF_COMM_PORTS_NUMBER_LEGAL, (Integer) jsonObject.getInt(Names.maxNumberRFCOMMPorts));
    }

    public void testNullDeserialization() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);
        assertNull(deviceInfo.getHardware());
        assertNull(deviceInfo.getFirmwareRev());
        assertNull(deviceInfo.getOS());
        assertNull(deviceInfo.getOSVersion());
        assertNull(deviceInfo.getCarrier());
        assertNull(deviceInfo.getMaxNumberRFCOMMPorts());
    }

    /**
     * Hardware section
     */

    public void testHardwareLegalViaSetter() {
        DeviceInfo deviceInfo = new DeviceInfo();
        deviceInfo.setHardware(HARDWARE_LEGAL);
        assertEquals(HARDWARE_LEGAL, deviceInfo.getHardware());
    }

    public void testHardwareLegalViaHashTable() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.hardware, HARDWARE_LEGAL);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);
        assertEquals(HARDWARE_LEGAL, deviceInfo.getHardware());
    }

    public void testRemoveHardware() {
        DeviceInfo deviceInfo = new DeviceInfo();
        deviceInfo.setHardware(HARDWARE_LEGAL);
        assertEquals(HARDWARE_LEGAL, deviceInfo.getHardware());

        deviceInfo.setHardware(null);
        assertNull(deviceInfo.getHardware());
    }

    public void testHardwareIllegalViaSetter() {
        DeviceInfo deviceInfo = new DeviceInfo();
        try {
            deviceInfo.setHardware(getRandomString(MAX_STRING_LENGTH * 2));
        } catch (IllegalArgumentException e) {
            /* Expected */
        }
    }

    public void testHardwareIllegalStringViaHashTable() throws JSONException {
        String illegalString = getRandomString(MAX_STRING_LENGTH * 2);

        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.hardware, illegalString);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);

        try {
            deviceInfo.getHardware();
        } catch (IllegalArgumentException e) {
            /* Expected */
        }
    }

    public void testHardwareIllegalNotStringViaHashTable() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.hardware, 1234567890);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);
        assertNull(deviceInfo.getHardware());
    }

    /**
     * FirmwareRev section
     */

    public void testFirmwareRevLegalViaSetter() {
        DeviceInfo deviceInfo = new DeviceInfo();
        deviceInfo.setFirmwareRev(FIRMWARE_REV_LEGAL);
        assertEquals(FIRMWARE_REV_LEGAL, deviceInfo.getFirmwareRev());
    }

    public void testFirmwareRevViaHashTable() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.firmwareRev, FIRMWARE_REV_LEGAL);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);
        assertEquals(FIRMWARE_REV_LEGAL, deviceInfo.getFirmwareRev());
    }

    public void testRemoveFirmwareRev() {
        DeviceInfo deviceInfo = new DeviceInfo();
        deviceInfo.setFirmwareRev(FIRMWARE_REV_LEGAL);
        assertEquals(FIRMWARE_REV_LEGAL, deviceInfo.getFirmwareRev());

        deviceInfo.setFirmwareRev(null);
        assertNull(deviceInfo.getFirmwareRev());
    }

    public void testFirmwareRevIllegalViaSetter() {
        DeviceInfo deviceInfo = new DeviceInfo();
        try {
            deviceInfo.setFirmwareRev(getRandomString(MAX_STRING_LENGTH * 2));
        } catch (IllegalArgumentException e) {
            /* Expected */
        }
    }

    public void testFirmwareRevIllegalStringViaHashTable() throws JSONException {
        String illegalString = getRandomString(MAX_STRING_LENGTH * 2);

        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.firmwareRev, illegalString);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);

        try {
            deviceInfo.getFirmwareRev();
        } catch (IllegalArgumentException e) {
            /* Expected */
        }
    }

    public void testFirmwareRevIllegalNotStringViaHashTable() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.firmwareRev, 1234567890);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);
        assertNull(deviceInfo.getFirmwareRev());
    }

    /**
     * OS section
     */

    public void testDeviceOSLegalViaSetter() {
        DeviceInfo deviceInfo = new DeviceInfo();
        deviceInfo.setOS(DEVICE_OS_LEGAL);
        assertEquals(DEVICE_OS_LEGAL, deviceInfo.getOS());
    }

    public void testDeviceOSViaHashTable() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.os, DEVICE_OS_LEGAL);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);
        assertEquals(DEVICE_OS_LEGAL, deviceInfo.getOS());
    }

    public void testRemoveDeviceOS() {
        DeviceInfo deviceInfo = new DeviceInfo();
        deviceInfo.setOS(DEVICE_OS_LEGAL);
        assertEquals(DEVICE_OS_LEGAL, deviceInfo.getOS());

        deviceInfo.setOS(null);
        assertNull(deviceInfo.getOS());
    }

    public void testDeviceOSIllegalViaSetter() {
        DeviceInfo deviceInfo = new DeviceInfo();
        try {
            deviceInfo.setOS(getRandomString(MAX_STRING_LENGTH * 2));
        } catch (IllegalArgumentException e) {
            /* Expected */
        }
    }

    public void testDeviceOSIllegalStringViaHashTable() throws JSONException {
        String illegalString = getRandomString(MAX_STRING_LENGTH * 2);

        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.os, illegalString);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);

        try {
            deviceInfo.getOS();
        } catch (IllegalArgumentException e) {
            /* Expected */
        }
    }

    public void testDeviceOSIllegalNotStringViaHashTable() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.os, 1234567890);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);
        assertNull(deviceInfo.getOS());
    }

    /**
     * OS Version section
     */

    public void testDeviceOSVersionLegalViaSetter() {
        DeviceInfo deviceInfo = new DeviceInfo();
        deviceInfo.setOSVersion(DEVICE_OS_VERSION_LEGAL);
        assertEquals(DEVICE_OS_VERSION_LEGAL, deviceInfo.getOSVersion());
    }

    public void testDeviceOSVersionViaHashTable() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.osVersion, DEVICE_OS_VERSION_LEGAL);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);
        assertEquals(DEVICE_OS_VERSION_LEGAL, deviceInfo.getOSVersion());
    }

    public void testRemoveDeviceOSVersion() {
        DeviceInfo deviceInfo = new DeviceInfo();
        deviceInfo.setOSVersion(DEVICE_OS_VERSION_LEGAL);
        assertEquals(DEVICE_OS_VERSION_LEGAL, deviceInfo.getOSVersion());

        deviceInfo.setOSVersion(null);
        assertNull(deviceInfo.getOSVersion());
    }

    public void testDeviceOSVersionIllegalViaSetter() {
        DeviceInfo deviceInfo = new DeviceInfo();
        try {
            deviceInfo.setOSVersion(getRandomString(MAX_STRING_LENGTH * 2));
        } catch (IllegalArgumentException e) {
            /* Expected */
        }
    }

    public void testDeviceOSVersionIllegalStringViaHashTable() throws JSONException {
        String illegalString = getRandomString(MAX_STRING_LENGTH * 2);

        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.osVersion, illegalString);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);

        try {
            deviceInfo.getOSVersion();
        } catch (IllegalArgumentException e) {
            /* Expected */
        }
    }

    public void testDeviceOSVersionIllegalNotStringViaHashTable() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.osVersion, 1234567890);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);
        assertNull(deviceInfo.getOSVersion());
    }

    /**
     * Carrier section
     */

    public void testCarrierVersionLegalViaSetter() {
        DeviceInfo deviceInfo = new DeviceInfo();
        deviceInfo.setCarrier(CARRIER_LEGAL);
        assertEquals(CARRIER_LEGAL, deviceInfo.getCarrier());
    }

    public void testCarrierViaHashTable() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.carrier, CARRIER_LEGAL);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);
        assertEquals(CARRIER_LEGAL, deviceInfo.getCarrier());
    }

    public void testRemoveCarrier() {
        DeviceInfo deviceInfo = new DeviceInfo();
        deviceInfo.setCarrier(CARRIER_LEGAL);
        assertEquals(CARRIER_LEGAL, deviceInfo.getCarrier());

        deviceInfo.setCarrier(null);
        assertNull(deviceInfo.getCarrier());
    }

    public void testCarrierIllegalViaSetter() {
        DeviceInfo deviceInfo = new DeviceInfo();
        try {
            deviceInfo.setCarrier(getRandomString(MAX_STRING_LENGTH * 2));
        } catch (IllegalArgumentException e) {
            /* Expected */
        }
    }

    public void testCarrierIllegalStringViaHashTable() throws JSONException {
        String illegalString = getRandomString(MAX_STRING_LENGTH * 2);

        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.carrier, illegalString);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);

        try {
            deviceInfo.getCarrier();
        } catch (IllegalArgumentException e) {
            /* Expected */
        }
    }

    public void testCarrierIllegalNotStringViaHashTable() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.carrier, 1234567890);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);
        assertNull(deviceInfo.getCarrier());
    }

    /**
     * Max Number RF COMM Ports section
     */

    public void testMaxNumberRFCOMMPortsLegalViaSetter() {
        DeviceInfo deviceInfo = new DeviceInfo();
        deviceInfo.setMaxNumberRFCOMMPorts(MAX_RF_COMM_PORTS_NUMBER_LEGAL);
        assertEquals(MAX_RF_COMM_PORTS_NUMBER_LEGAL, deviceInfo.getMaxNumberRFCOMMPorts());
    }

    public void testMaxNumberRFCOMMPortsViaHashTable() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.maxNumberRFCOMMPorts, MAX_RF_COMM_PORTS_NUMBER_LEGAL);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);
        assertEquals(MAX_RF_COMM_PORTS_NUMBER_LEGAL, deviceInfo.getMaxNumberRFCOMMPorts());
    }

    public void testRemoveMaxNumberRFCOMMPorts() {
        DeviceInfo deviceInfo = new DeviceInfo();
        deviceInfo.setMaxNumberRFCOMMPorts(MAX_RF_COMM_PORTS_NUMBER_LEGAL);
        assertEquals(MAX_RF_COMM_PORTS_NUMBER_LEGAL, deviceInfo.getMaxNumberRFCOMMPorts());

        deviceInfo.setMaxNumberRFCOMMPorts(null);
        assertNull(deviceInfo.getMaxNumberRFCOMMPorts());
    }

    public void testMaxNumberRFCOMMPortsIllegalViaSetter() {
        DeviceInfo deviceInfo = new DeviceInfo();
        try {
            deviceInfo.setMaxNumberRFCOMMPorts(MAX_INT_LENGTH * 2);
        } catch (IllegalArgumentException e) {
            /* Expected */
        }
    }

    public void testMaxNumberRFCOMMPortsIllegalIntViaHashTable() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.maxNumberRFCOMMPorts, MAX_INT_LENGTH * 2);

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);

        try {
            deviceInfo.getMaxNumberRFCOMMPorts();
        } catch (IllegalArgumentException e) {
            /* Expected */
        }
    }

    public void testMaxNumberRFCOMMPortsIllegalNotIntViaHashTable() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.maxNumberRFCOMMPorts, "1234567890");

        DeviceInfo deviceInfo = new DeviceInfo(JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(deviceInfo);
        assertNull(deviceInfo.getMaxNumberRFCOMMPorts());
    }

    private String getRandomString(int stringLength) {
        Random generator = new Random();
        StringBuilder randomStringBuilder = new StringBuilder();
        int randomLength = generator.nextInt(stringLength);
        char tempChar;
        for (int i = 0; i < randomLength; i++) {
            tempChar = (char) (generator.nextInt(96) + 32);
            randomStringBuilder.append(tempChar);
        }
        return randomStringBuilder.toString();
    }

    public static DeviceInfo getDeviceInfo() {
        DeviceInfo deviceInfo = new DeviceInfo();
        deviceInfo.setHardware(HARDWARE_LEGAL);
        deviceInfo.setFirmwareRev(FIRMWARE_REV_LEGAL);
        deviceInfo.setOS(DEVICE_OS_LEGAL);
        deviceInfo.setOSVersion(DEVICE_OS_VERSION_LEGAL);
        deviceInfo.setCarrier(CARRIER_LEGAL);
        deviceInfo.setMaxNumberRFCOMMPorts(MAX_RF_COMM_PORTS_NUMBER_LEGAL);
        return deviceInfo;
    }
}