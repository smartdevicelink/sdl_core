package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.FrameDataControlFrameType;
import com.ford.syncV4.protocol.enums.FrameType;
import com.ford.syncV4.protocol.enums.ServiceType;

public class ProtocolFrameHeaderFactory {


    public static ProtocolFrameHeader createMobileNavStartSession(int messageID, byte sessionID, byte version) {
        ProtocolFrameHeader msg = new ProtocolFrameHeader();
        msg.setVersion(version);
        msg.setFrameType(FrameType.Control);
        msg.setServiceType(ServiceType.Mobile_Nav);
        msg.setFrameData(FrameDataControlFrameType.StartService.value());
        msg.setMessageID(messageID);
        msg.setSessionID(sessionID);
        msg.setCompressed(false);
        msg.setDataSize(0);
        return msg;
    }

    public static ProtocolFrameHeader createStartSession(ServiceType serviceType, byte sessionID, byte version) {
        ProtocolFrameHeader msg = new ProtocolFrameHeader();
        msg.setVersion(version);
        msg.setFrameType(FrameType.Control);
        msg.setServiceType(serviceType);
        msg.setFrameData(FrameDataControlFrameType.StartService.value());
        msg.setMessageID(0);
        msg.setSessionID(sessionID);

        return msg;
    }

    public static ProtocolFrameHeader createStartSessionACK(ServiceType serviceType, byte sessionID, int messageID, byte version) {
        ProtocolFrameHeader msg = new ProtocolFrameHeader();
        msg.setVersion(version);
        msg.setFrameType(FrameType.Control);
        msg.setServiceType(serviceType);
        msg.setSessionID(sessionID);
        msg.setFrameData(FrameDataControlFrameType.StartServiceACK.value());
        msg.setMessageID(messageID);

        return msg;
    }

    public static ProtocolFrameHeader createStartSessionNACK(ServiceType serviceType, byte sessionID, int messageID, byte version) {
        ProtocolFrameHeader msg = new ProtocolFrameHeader();
        msg.setVersion(version);
        msg.setFrameType(FrameType.Control);
        msg.setServiceType(serviceType);
        msg.setSessionID(sessionID);
        msg.setFrameData(FrameDataControlFrameType.StartServiceNACK.value());
        msg.setMessageID(messageID);

        return msg;
    }

    public static ProtocolFrameHeader createEndSession(ServiceType serviceType,
                                                       byte sessionID, int messageID,
                                                       byte version,
                                                       int dataLength) {
        ProtocolFrameHeader msg = new ProtocolFrameHeader();
        msg.setVersion(version);
        msg.setFrameType(FrameType.Control);
        msg.setServiceType(serviceType);
        msg.setSessionID(sessionID);
        msg.setFrameData(FrameDataControlFrameType.EndService.value());
        msg.setMessageID(messageID);
        msg.setDataSize(dataLength);

        return msg;
    }

    public static ProtocolFrameHeader createSingleSendData(ServiceType serviceType, byte sessionID,
                                                           int dataLength, int messageID, byte version) {
        ProtocolFrameHeader msg = new ProtocolFrameHeader();
        msg.setVersion(version);
        msg.setFrameType(FrameType.Single);
        msg.setServiceType(serviceType);
        msg.setFrameData(ProtocolFrameHeader.FrameDataSingleFrame);
        msg.setSessionID(sessionID);
        msg.setDataSize(dataLength);
        msg.setMessageID(messageID);

        return msg;
    }

    public static ProtocolFrameHeader createMultiSendDataFirst(ServiceType serviceType, byte sessionID,
                                                               int messageID, byte version) {
        ProtocolFrameHeader msg = new ProtocolFrameHeader();
        msg.setVersion(version);
        msg.setFrameType(FrameType.First);
        msg.setServiceType(serviceType);
        msg.setFrameData(ProtocolFrameHeader.FrameDataFirstFrame);
        msg.setSessionID(sessionID);
        msg.setDataSize(8);
        msg.setMessageID(messageID);

        return msg;
    }

    public static ProtocolFrameHeader createMultiSendDataRest(ServiceType serviceType, byte sessionID,
                                                              int dataLength, byte frameSequenceNumber, int messageID, byte version) {
        ProtocolFrameHeader msg = new ProtocolFrameHeader();
        msg.setVersion(version);
        msg.setFrameType(FrameType.Consecutive);
        msg.setServiceType(serviceType);
        msg.setFrameData(frameSequenceNumber/*FrameData.ConsecutiveFrame.value()*/);
        msg.setSessionID(sessionID);
        msg.setDataSize(dataLength);
        msg.setMessageID(messageID);

        return msg;
    }

    public static BinaryFrameHeader createBinaryFrameHeader(byte rpcType, int functionID, int corrID, int jsonSize) {
        BinaryFrameHeader msg = new BinaryFrameHeader();
        msg.setRPCType(rpcType);
        msg.setFunctionID(functionID);
        msg.setCorrID(corrID);
        msg.setJsonSize(jsonSize);

        return msg;
    }

    public static ProtocolFrameHeader createMobileNaviAck(ServiceType serviceType, byte sessionID,
                                                          int consumedFramesNumber, byte version) {
        ProtocolFrameHeader msg = new ProtocolFrameHeader();
        msg.setVersion(version);
        msg.setFrameType(FrameType.Control);
        msg.setServiceType(serviceType);
        msg.setSessionID(sessionID);
        msg.setFrameData(FrameDataControlFrameType.MobileNaviACK.value());
        msg.setMessageID(consumedFramesNumber);
        return msg;
    }

    public static ProtocolFrameHeader createHeartbeat(ServiceType serviceType, byte version) {
        return createControlFrame(serviceType, version, FrameDataControlFrameType.Heartbeat);
    }

    public static ProtocolFrameHeader createHeartbeatACK(ServiceType serviceType, byte version) {
        return createControlFrame(serviceType, version, FrameDataControlFrameType.HeartbeatACK);
    }

    private static ProtocolFrameHeader createControlFrame(ServiceType serviceType, byte version,
                                                          FrameDataControlFrameType frameData) {
        ProtocolFrameHeader msg = new ProtocolFrameHeader();
        msg.setVersion(version);
        msg.setFrameType(FrameType.Control);
        msg.setServiceType(serviceType);
        msg.setFrameData(frameData.value());
        return msg;
    }
}