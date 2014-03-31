package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.constants.Names;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/25/14
 * Time: 11:55 AM
 */
public class RegisterAppInterfaceTest extends TestCase {

    private static final String TAG = "RegisterAppInterfaceTest";

    private String mLegalHashId_Empty = "";
    private String mLegalHashId_Digits = "1234567890";
    private String mLegalHashId_Letters = "qwertyuiop";
    private String mLegalHashId_Symbols = "!@#$%^&*(){}><?/|_";
    private String mIllegalHashId = "dG8f5887951261W2a578o4L65X484eVl38tWr1n5474727m9iF6n2p" +
            "620335PM18z322B28Q3XG38257kY5Pn8n3584596rU7J8ldG8f5887951261W2a578o4L65X484eVl" +
            "38tWr1n5474727m9iF6n2p620335PM18z322B28Q3XG38257kY5Pn8n3584596rU7J8l";

    public void testSetLegalHashId_Empty() {
        RegisterAppInterface registerAppInterface = new RegisterAppInterface();
        registerAppInterface.setHashID(mLegalHashId_Empty);
        assertEquals(registerAppInterface.getHashID(), mLegalHashId_Empty);
    }

    public void testSetLegalHashId_Digits() {
        RegisterAppInterface registerAppInterface = new RegisterAppInterface();
        registerAppInterface.setHashID(mLegalHashId_Digits);
        assertEquals(registerAppInterface.getHashID(), mLegalHashId_Digits);
    }

    public void testSetLegalHashId_Letters() {
        RegisterAppInterface registerAppInterface = new RegisterAppInterface();
        registerAppInterface.setHashID(mLegalHashId_Letters);
        assertEquals(registerAppInterface.getHashID(), mLegalHashId_Letters);
    }

    public void testSetLegalHashId_Symbols() {
        RegisterAppInterface registerAppInterface = new RegisterAppInterface();
        registerAppInterface.setHashID(mLegalHashId_Symbols);
        assertEquals(registerAppInterface.getHashID(), mLegalHashId_Symbols);
    }

    public void testSetLegalHashId_Empty_WithConstructor() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.hashID, mLegalHashId_Empty);

        RegisterAppInterface registerAppInterface = new RegisterAppInterface(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertEquals(registerAppInterface.getHashID(), mLegalHashId_Empty);
    }

    public void testSetLegalHashId_Digits_WithConstructor() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.hashID, mLegalHashId_Digits);

        RegisterAppInterface registerAppInterface = new RegisterAppInterface(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertEquals(registerAppInterface.getHashID(), mLegalHashId_Digits);
    }

    public void testSetLegalHashId_Letters_WithConstructor() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.hashID, mLegalHashId_Letters);

        RegisterAppInterface registerAppInterface = new RegisterAppInterface(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertEquals(registerAppInterface.getHashID(), mLegalHashId_Letters);
    }

    public void testSetLegalHashId_Symbols_WithConstructor() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.hashID, mLegalHashId_Symbols);

        RegisterAppInterface registerAppInterface = new RegisterAppInterface(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertEquals(registerAppInterface.getHashID(), mLegalHashId_Symbols);
    }

    public void testSetIllegalHashId_Symbols() {
        RegisterAppInterface registerAppInterface = new RegisterAppInterface();
        try {
            registerAppInterface.setHashID(mIllegalHashId);
            fail();
        } catch (IllegalArgumentException e) {
            /* expected */
        }
    }

    public void testRemoveHashId() {
        RegisterAppInterface registerAppInterface = new RegisterAppInterface();
        registerAppInterface.setHashID(mLegalHashId_Digits);
        assertEquals(registerAppInterface.getHashID(), mLegalHashId_Digits);

        registerAppInterface.setHashID(null);
        assertNull(registerAppInterface.getHashID());
    }

    public void testSetLegalDeviceInfo() {
        DeviceInfo deviceInfo = DeviceInfoTest.getDeviceInfo();

        RegisterAppInterface registerAppInterface = new RegisterAppInterface();
        registerAppInterface.setDeviceInfo(deviceInfo);
        assertEquals(registerAppInterface.getDeviceInfo(), deviceInfo);
    }

    public void testSetLegalDeviceInfo_WithConstructor() throws JSONException {
        DeviceInfo deviceInfo = DeviceInfoTest.getDeviceInfo();

        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.deviceInfo, deviceInfo);

        RegisterAppInterface registerAppInterface = new RegisterAppInterface(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertEquals(registerAppInterface.getDeviceInfo(), deviceInfo);
    }

    public void testRemoveDeviceInfo() {
        DeviceInfo deviceInfo = DeviceInfoTest.getDeviceInfo();

        RegisterAppInterface registerAppInterface = new RegisterAppInterface();
        registerAppInterface.setDeviceInfo(deviceInfo);
        assertEquals(registerAppInterface.getDeviceInfo(), deviceInfo);

        registerAppInterface.setDeviceInfo(null);
        assertNull(registerAppInterface.getDeviceInfo());
    }

    public void testSetIllegalDeviceInfo_WithConstructor() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.deviceInfo, 1234567890);

        RegisterAppInterface registerAppInterface = new RegisterAppInterface(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertNull(registerAppInterface.getDeviceInfo());
    }
}