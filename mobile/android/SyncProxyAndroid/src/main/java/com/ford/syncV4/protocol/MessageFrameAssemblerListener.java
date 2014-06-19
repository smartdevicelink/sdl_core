package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.ServiceType;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 18.06.14
 * Time: 13:20
 */

/**
 * This interface describes a methods which {@link com.ford.syncV4.protocol.MessageFrameAssembler}
 * must use to provide callbacks of its logic.
 */
public interface MessageFrameAssemblerListener {

    /**
     * A method, which indicates that
     * {@link com.ford.syncV4.protocol.enums.FrameDataControlFrameType#HeartbeatACK} has been
     * received
     *
     * @param sessionId Identifier of the current SYNC Session
     */
    public void onHeartbeatACK(byte sessionId);

    /**
     * A method, which indicates that
     * {@link com.ford.syncV4.protocol.enums.FrameDataControlFrameType#Heartbeat} has been
     * received
     *
     * @param sessionId Identifier of the current SYNC Session
     */
    public void onHeartbeat(byte sessionId);

    /**
     * A method, which indicates that
     * {@link com.ford.syncV4.protocol.enums.FrameDataControlFrameType#StartService} has been
     * received
     *
     * @param sessionId   Identifier of the current SYNC Session
     * @param serviceType Type of the Service
     */
    public void onStartService(byte sessionId, ServiceType serviceType);

    /**
     * A method, which indicates that
     * {@link com.ford.syncV4.protocol.enums.FrameDataControlFrameType#StartServiceACK} has been
     * received
     *
     * @param sessionId       Identifier of the current SYNC Session
     * @param messageId       Identifier of the message
     * @param serviceType     Type of the Service
     * @param protocolVersion Version of the protocol, returned by the SDL
     */
    public void onStartServiceACK(byte sessionId, int messageId, ServiceType serviceType,
                                  byte protocolVersion);

    /**
     * A method, which indicates that
     * {@link com.ford.syncV4.protocol.enums.FrameDataControlFrameType#StartServiceNACK} has been
     * received
     *
     * @param sessionId   Identifier of the current SYNC Session
     * @param serviceType Type of the Service
     */
    public void onStartServiceNACK(byte sessionId, ServiceType serviceType);

    /**
     * A method, which indicates that
     * {@link com.ford.syncV4.protocol.enums.FrameDataControlFrameType#EndService} has been
     * received
     *
     * @param sessionId   Identifier of the current SYNC Session
     * @param messageId   Identifier of the message
     * @param serviceType Type of the Service
     */
    public void onEndService(byte sessionId, int messageId, ServiceType serviceType);

    /**
     * A method, which indicates that
     * {@link com.ford.syncV4.protocol.enums.FrameDataControlFrameType#EndServiceNACK} has been
     * received
     *
     * @param sessionId Identifier of the current SYNC Session
     */
    public void onEndServiceNACK(byte sessionId);

    /**
     * A method, which indicates that
     * {@link com.ford.syncV4.protocol.enums.FrameDataControlFrameType#MobileNaviACK} has been
     * received
     *
     * @param sessionId Identifier of the current SYNC Session
     * @param messageId Identifier of the message
     */
    public void onMobileNaviACK(byte sessionId, int messageId);

    /**
     * A method, which indicates that
     * {@link com.ford.syncV4.protocol.enums.FrameDataControlFrameType#EndServiceACK} has been
     * received
     *
     * @param sessionId   Identifier of the current SYNC Session
     * @param messageId   Identifier of the message
     * @param serviceType Type of the Service
     */
    public void onEndServiceACK(byte sessionId, int messageId, ServiceType serviceType);

    /**
     * A method, which indicates that {@link com.ford.syncV4.protocol.ProtocolMessage} has been
     * received
     *
     * @param messageId       Identifier of the message
     * @param protocolMessage {@link com.ford.syncV4.protocol.ProtocolMessage}
     */
    public void onHandleProtocolMessageReceived(int messageId, ProtocolMessage protocolMessage);

    /**
     * help to handle application un-registration
     */
    public void onHandleAppUnregistered();
}