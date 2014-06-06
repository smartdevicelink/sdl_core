package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.session.Session;

import junit.framework.TestCase;

/**
 * Created by u on 2013-09-30.
 */
public class WiProProtocolUSBTest extends TestCase {
    private boolean requiredMethodCalled;

    public void testUnregisterAppInterfaceResponseCallsProtocolListener() {
        final IProtocolListener protocolListener = new IProtocolListener() {
            @Override
            public void onProtocolMessageBytesToSend(byte[] msgBytes,
                                                     int offset, int length) {
            }

            @Override
            public void onProtocolMessageReceived(ProtocolMessage msg) {
            }

            @Override
            public void onProtocolSessionStarted(byte sessionId,
                                                 byte version) {
            }

            @Override
            public void onProtocolServiceEnded(ServiceType sessionType,
                                               byte sessionID) {
            }

            @Override
            public void onProtocolHeartbeatACK(byte sessionId) {
            }

            @Override
            public void onProtocolHeartbeat(byte sessionId) {

            }

            @Override
            public void onResetHeartbeatAck(byte sessionId) {

            }

            @Override
            public void onProtocolError(String info, Exception e) {
            }

            @Override
            public void onMobileNavAckReceived(byte sessionId, int frameReceivedNumber) {
            }

            @Override
            public void onProtocolAppUnregistered() {
                requiredMethodCalled = true;
            }

            @Override
<<<<<<< HEAD
            public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, boolean encrypted, byte version, String correlationID) {
=======
            public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, byte version) {
>>>>>>> develop

            }

            @Override
            public void onStartServiceNackReceived(byte sessionId, ServiceType serviceType) {

            }

            @Override
            public void onResetHeartbeat(byte sessionId) {

            }

            @Override
            public void onProtocolServiceEndedAck(ServiceType serviceType, byte sessionId) {

            }

        };

        requiredMethodCalled = false;
        final WiProProtocol protocol = new WiProProtocol(protocolListener);
        protocol.setProtocolVersion(ProtocolConstants.PROTOCOL_VERSION_MAX);

        byte[] unregisterAppInterfaceResponse = new byte[]{
                // first bytes
                0x21, 0x07, 0x00, 0x01,
                // data size
                0x00, 0x00, 0x00, 14,
                // message ID
                0x00, 0x00, 0x00, 0x01,
                // RPC type and function ID
                0x10, 0x00, 0x00, 0x02,
                // correlation ID
                0x00, 0x00, 0x00, 0x01,
                // JSON size
                0x00, 0x00, 0x00, 0x02,
                // JSON
                '{', '}'
        };

        protocol.HandleReceivedBytes(unregisterAppInterfaceResponse,
                unregisterAppInterfaceResponse.length);
        assertTrue("onProtocolAppUnregistered() method isn't called",
                requiredMethodCalled);
    }

    public void testUnregisterAppInterfaceRequestDoesntCallProtocolListener() {
        final IProtocolListener protocolListener = new IProtocolListener() {
            @Override
            public void onProtocolMessageBytesToSend(byte[] msgBytes,
                                                     int offset, int length) {
            }

            @Override
            public void onProtocolMessageReceived(ProtocolMessage msg) {
            }

            @Override
            public void onProtocolSessionStarted(byte sessionId,
                                                 byte version) {
            }

            @Override
            public void onProtocolServiceEnded(ServiceType sessionType,
                                               byte sessionID) {
            }

            @Override
            public void onProtocolHeartbeatACK(byte sessionId) {
            }

            @Override
            public void onProtocolHeartbeat(byte sessionId) {

            }

            @Override
            public void onResetHeartbeatAck(byte sessionId) {

            }

            @Override
            public void onProtocolError(String info, Exception e) {
            }

            @Override
            public void onMobileNavAckReceived(byte sessionId, int frameReceivedNumber) {
            }

            @Override
            public void onProtocolAppUnregistered() {
                assertFalse(
                        "Incoming UnregisterAppInterface request shouldn't " +
                                "trigger the method", true);
            }

            @Override
<<<<<<< HEAD
            public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, boolean encrypted, byte version, String correlationID) {
=======
            public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, byte version) {

            }

            @Override
            public void onStartServiceNackReceived(byte sessionId, ServiceType serviceType) {
>>>>>>> develop

            }

            @Override
            public void onResetHeartbeat(byte sessionId) {

            }

            @Override
            public void onProtocolServiceEndedAck(ServiceType serviceType, byte sessionId) {

            }

        };

        final WiProProtocol protocol = new WiProProtocol(protocolListener);
        protocol.setProtocolVersion(ProtocolConstants.PROTOCOL_VERSION_MAX);

        byte[] unregisterAppInterfaceRequest = new byte[]{
                // first bytes
                0x21, 0x07, 0x00, 0x01,
                // data size
                0x00, 0x00, 0x00, 14,
                // message ID
                0x00, 0x00, 0x00, 0x01,
                // RPC type and function ID
                0x00, 0x00, 0x00, 0x02,
                // correlation ID
                0x00, 0x00, 0x00, 0x01,
                // JSON size
                0x00, 0x00, 0x00, 0x02,
                // JSON
                '{', '}'
        };

        protocol.HandleReceivedBytes(unregisterAppInterfaceRequest,
                unregisterAppInterfaceRequest.length);
    }
}
