package com.ford.syncV4.util;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/12/14
 * Time: 10:31 AM
 */
public class CommonUtilsTest extends TestCase {

    public void testIsUSBNoSuchDeviceError_True() {
        String mErrorMessage = "java.io.IOException: write failed: ENODEV (No such device)";
        assertTrue(CommonUtils.isUSBNoSuchDeviceError(mErrorMessage));

        mErrorMessage = "IOException: write ENODEV (No such device";
        assertTrue(CommonUtils.isUSBNoSuchDeviceError(mErrorMessage));

        mErrorMessage = "IOExceptionENODEVNo such device";
        assertTrue(CommonUtils.isUSBNoSuchDeviceError(mErrorMessage));

        mErrorMessage = "Transport failure: Failed to send bytes over USB\n" +
                "    java.io.IOException: write failed: ENODEV (No such device)\n" +
                "            at libcore.io.IoBridge.write(IoBridge.java:506)\n" +
                "            at java.io.FileOutputStream.write(FileOutputStream.java:187)\n" +
                "            at com.ford.syncV4.transport.usb.USBTransport.sendBytesOverTransport(USBTransport.java:231)\n" +
                "            at com.ford.syncV4.transport.SyncTransport.sendBytes(SyncTransport.java:63)\n" +
                "            at com.ford.syncV4.syncConnection.SyncConnection.onProtocolMessageBytesToSend(SyncConnection.java:372)\n" +
                "            at com.ford.syncV4.protocol.AbstractProtocol.handleProtocolMessageBytesToSend(AbstractProtocol.java:177)\n" +
                "            at com.ford.syncV4.protocol.AbstractProtocol.handleProtocolFrameToSend(AbstractProtocol.java:93)\n" +
                "            at com.ford.syncV4.protocol.WiProProtocol.sendFrameToTransport(WiProProtocol.java:171)\n" +
                "            at com.ford.syncV4.protocol.WiProProtocol.StartProtocolSession(WiProProtocol.java:66)\n" +
                "            at com.ford.syncV4.syncConnection.SyncConnection.startProtocolSession(SyncConnection.java:336)\n" +
                "            at com.ford.syncV4.syncConnection.SyncConnection.initialiseSession(SyncConnection.java:329)\n" +
                "            at com.ford.syncV4.syncConnection.SyncConnection.onTransportConnected(SyncConnection.java:322)\n" +
                "            at com.ford.syncV4.transport.SyncTransport.handleTransportConnected(SyncTransport.java:80)\n" +
                "            at com.ford.syncV4.transport.usb.USBTransport.setState(USBTransport.java:205)\n" +
                "            at com.ford.syncV4.transport.usb.USBTransport$USBTransportReader.connect(USBTransport.java:691)\n" +
                "            at com.ford.syncV4.transport.usb.USBTransport$USBTransportReader.run(USBTransport.java:649)\n" +
                "            at java.lang.Thread.run(Thread.java:838)\n" +
                "     Caused by: libcore.io.ErrnoException: write failed: ENODEV (No such device)";
        assertTrue(CommonUtils.isUSBNoSuchDeviceError(mErrorMessage));
    }

    public void testIsUSBNoSuchDeviceError_False() {
        String mErrorMessage = "java.io.IOException: write failed: (No such device)";
        assertFalse(CommonUtils.isUSBNoSuchDeviceError(mErrorMessage));

        mErrorMessage = "java.io.IOException: write failed: ENODEV (No such )";
        assertFalse(CommonUtils.isUSBNoSuchDeviceError(mErrorMessage));

        mErrorMessage = "";
        assertFalse(CommonUtils.isUSBNoSuchDeviceError(mErrorMessage));

        mErrorMessage = null;
        assertFalse(CommonUtils.isUSBNoSuchDeviceError(mErrorMessage));
    }

    public static JSONObject paramsToRequestObject(JSONObject paramsObject) throws JSONException {
        JSONObject jsonObject = new JSONObject();
        JSONObject requestObject = new JSONObject();
        jsonObject.put("request", requestObject);
        requestObject.put("parameters", paramsObject);
        return jsonObject;
    }
}