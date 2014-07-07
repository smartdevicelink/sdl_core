package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.FrameDataControlFrameType;
import com.ford.syncV4.protocol.enums.FrameType;
import com.ford.syncV4.protocol.enums.FunctionID;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.util.BitConverter;
import com.ford.syncV4.util.logger.Logger;

import java.io.ByteArrayOutputStream;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 18.06.14
 * Time: 13:06
 */

/**
 * This class provides methods to assemble message frames from bytes array, received from the
 * transport
 */
public class MessageFrameAssembler {
    
    private static final String CLASS_NAME = MessageFrameAssembler.class.getSimpleName();

    /**
     * Listener, to provide callbacks of the assemble message logic
     */
    private MessageFrameAssemblerListener mMessageFrameAssemblerListener;

    private ByteArrayOutputStream accumulator = null;

    /**
     * Constructor
     *
     * @param listener {@link com.ford.syncV4.protocol.MessageFrameAssemblerListener}
     */
    public MessageFrameAssembler(MessageFrameAssemblerListener listener) {
        if (listener == null) {
            throw new NullPointerException("MessageFrameAssemblerListener can not be null");
        }
        mMessageFrameAssemblerListener = listener;
    }

    /**
     * Handle incoming bytes array
     *
     * @param header {@link com.ford.syncV4.protocol.ProtocolFrameHeader}
     * @param data   bytes array
     */
    public void handleFrame(ProtocolFrameHeader header, byte[] data, int frameHeaderSize) {
        if (header.getFrameType().equals(FrameType.Control)) {
            handleControlFrame(header);
        } else {
            // Must be a form of data frame (single, first, consecutive, etc.)
            if (header.getFrameType() == FrameType.First ||
                    header.getFrameType() == FrameType.Consecutive) {
                handleMultiFrameMessageFrame(header, data, frameHeaderSize);
            } else {
                handleSingleFrameMessageFrame(header, data);
            }
        }
    }

    /**
     * Handle first frame of the message
     *
     * @param data   bytes array
     */
    private void handleFirstDataFrame(byte[] data, int frameHeaderSize) {
        //The message is new, so let's figure out how big it is.
        accumulator = new ByteArrayOutputStream(BitConverter.intFromByteArray(data, 0) -
                frameHeaderSize);
    }

    /**
     * Handle remaining frames
     *
     * @param header {@link com.ford.syncV4.protocol.ProtocolFrameHeader}
     * @param data   bytes array
     */
    private void handleRemainingFrame(ProtocolFrameHeader header, byte[] data) {
        accumulator.write(data, 0, header.getDataSize());
        notifyIfFinished(header);
    }

    private void notifyIfFinished(ProtocolFrameHeader header) {
        if (header.getFrameType() == FrameType.Consecutive && header.getFrameData() == 0x0) {
            ProtocolMessage message = new ProtocolMessage();
            message.setServiceType(header.getServiceType());
            message.setSessionID(header.getSessionId());
            //If it is WiPro 2.0 it must have binary header
            if (header.getVersion() >= ProtocolConstants.PROTOCOL_VERSION_TWO) {
                BinaryFrameHeader binFrameHeader = BinaryFrameHeader.
                        parseBinaryHeader(accumulator.toByteArray());
                message.setVersion(header.getVersion());
                message.setRPCType(binFrameHeader.getRPCType());
                message.setFunctionID(binFrameHeader.getFunctionID());
                message.setCorrID(binFrameHeader.getCorrID());
                if (binFrameHeader.getJsonSize() > 0)
                    message.setData(binFrameHeader.getJsonData());
                if (binFrameHeader.getBulkData() != null) {
                    message.setBulkData(binFrameHeader.getBulkData());
                }
            } else {
                message.setData(accumulator.toByteArray());
            }

            mMessageFrameAssemblerListener.onHandleProtocolMessageReceived(header.getMessageID(),
                    message);

            accumulator = null;
        }
    }

    /**
     * Handle multi-frame message
     *
     * @param header {@link com.ford.syncV4.protocol.ProtocolFrameHeader}
     * @param data   bytes array
     */
    private void handleMultiFrameMessageFrame(ProtocolFrameHeader header, byte[] data,
                                              int frameHeaderSize) {
        if (header.getFrameType() == FrameType.First) {
            handleFirstDataFrame(data, frameHeaderSize);
        } else {
            handleRemainingFrame(header, data);
        }
    }

    /**
     * Handle control frame
     *
     * @param header {@link com.ford.syncV4.protocol.ProtocolFrameHeader}
     */
    private void handleControlFrame(ProtocolFrameHeader header) {
        byte frameData = header.getFrameData();
        byte sessionId = header.getSessionId();
        int messageId = header.getMessageID();
        ServiceType serviceType = header.getServiceType();
        //Logger.d(CLASS_NAME + " incoming control frame:" + header.toString());

        // TODO : Implement getServiceType check point for Heartbeat and other non RPC's

        if (frameData == FrameDataControlFrameType.HeartbeatACK.getValue()) {
            mMessageFrameAssemblerListener.onHeartbeatACK(sessionId);
        } else if (frameData == FrameDataControlFrameType.Heartbeat.getValue()) {
            mMessageFrameAssemblerListener.onHeartbeat(sessionId);
        } else if (frameData == FrameDataControlFrameType.StartService.getValue()) {
            mMessageFrameAssemblerListener.onStartService(sessionId, serviceType);
        } else if (frameData == FrameDataControlFrameType.StartServiceACK.getValue()) {
            mMessageFrameAssemblerListener.onStartServiceACK(sessionId, messageId, serviceType,
                    header.getVersion());
        } else if (frameData == FrameDataControlFrameType.StartServiceNACK.getValue()) {
            mMessageFrameAssemblerListener.onStartServiceNACK(sessionId, serviceType);
        } else if (frameData == FrameDataControlFrameType.EndService.getValue()) {
            mMessageFrameAssemblerListener.onEndService(sessionId, messageId, serviceType);
        } else if (frameData == FrameDataControlFrameType.EndServiceNACK.getValue()) {
            mMessageFrameAssemblerListener.onEndServiceNACK(sessionId);
        } else if (header.getServiceType().getValue() == ServiceType.Mobile_Nav.getValue() &&
                frameData == FrameDataControlFrameType.MobileNaviACK.getValue()) {
            mMessageFrameAssemblerListener.onMobileNaviACK(sessionId, messageId);
        } else if (frameData == FrameDataControlFrameType.EndServiceACK.getValue()) {
            mMessageFrameAssemblerListener.onEndServiceACK(sessionId, messageId, serviceType);
        }
    }

    /**
     * Handle single frame message frame
     *
     * @param header {@link com.ford.syncV4.protocol.ProtocolFrameHeader}
     * @param data   bytes array
     */
    private void handleSingleFrameMessageFrame(ProtocolFrameHeader header, byte[] data) {
        ProtocolMessage message = new ProtocolMessage();
        if (header.getServiceType() == ServiceType.RPC) {
            message.setMessageType(MessageType.RPC);
        } else if (header.getServiceType() == ServiceType.Bulk_Data) {
            message.setMessageType(MessageType.BULK);
        }
        message.setServiceType(header.getServiceType());
        message.setSessionID(header.getSessionId());
        //If it is WiPro 2.0 it must have binary header
        if (header.getVersion() >= ProtocolConstants.PROTOCOL_VERSION_TWO) {
            BinaryFrameHeader binFrameHeader = BinaryFrameHeader.parseBinaryHeader(data);
            message.setVersion(header.getVersion());
            message.setRPCType(binFrameHeader.getRPCType());
            message.setFunctionID(binFrameHeader.getFunctionID());
            message.setCorrID(binFrameHeader.getCorrID());
            if (binFrameHeader.getJsonSize() > 0) message.setData(binFrameHeader.getJsonData());
            if (binFrameHeader.getBulkData() != null) {
                message.setBulkData(binFrameHeader.getBulkData());
            }
        } else {
            message.setData(data);
        }

        if (isAppUnregistered(message)) {
            mMessageFrameAssemblerListener.onHandleAppUnregistered();
        }

        mMessageFrameAssemblerListener.onHandleProtocolMessageReceived(header.getMessageID(),
                message);
    }

    private boolean isAppUnregistered(ProtocolMessage message) {
        return (message.getRPCType() == ProtocolMessage.RPCTYPE_RESPONSE) &&
                (message.getFunctionID() == FunctionID.getFunctionID(Names.UnregisterAppInterface));
    }
}