package com.ford.syncV4.protocol;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.protocol.enums.FrameDataControlFrameType;
import com.ford.syncV4.protocol.enums.FrameType;
import com.ford.syncV4.protocol.enums.FunctionID;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.util.BitConverter;
import com.ford.syncV4.util.logger.Logger;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.Hashtable;

public class WiProProtocol extends AbstractProtocol {

    private final static String FailurePropagating_Msg = "Failure propagating ";
    private ProtocolVersion mProtocolVersion = new ProtocolVersion();


    boolean _haveHeader = false;
    int _headerBufWritePos = 0;
    ProtocolFrameHeader _currentHeader = null;
    byte[] _dataBuf = null;
    int _dataBufWritePos = 0;
    int hashID = 0;

    protected Hashtable<Byte, Object> _messageLocks = new Hashtable<Byte, Object>();

    private static final String CLASS_NAME = WiProProtocol.class.getSimpleName();
    private static final String FAILURE_PROPAGATING_MSG = "Failure propagating ";
    private static final Hashtable<Integer, MessageFrameAssembler> ASSEMBLER_FOR_MESSAGE_ID =
            new Hashtable<Integer, MessageFrameAssembler>();
    private static final Hashtable<Byte, Hashtable<Integer, MessageFrameAssembler>> ASSEMBLER_FOR_SESSION_ID =
            new Hashtable<Byte, Hashtable<Integer, MessageFrameAssembler>>();

    private final SendProtocolMessageProcessor sendProtocolMessageProcessor =
            new SendProtocolMessageProcessor();

    private ProtocolFrameHeader mCurrentHeader = null;
    // NOTE: To date, not implemented on SYNC
    private int _heartbeatSendInterval_ms = 0;
    // NOTE: To date, not implemented on SYNC
    private int _heartbeatReceiveInterval_ms = 0;
    private boolean mHaveHeader = false;
    private byte[] mDataBuf = null;
    private int mDataBufWritePos = 0;
    private int mMessageID = 0;


    // Hide no-arg ctor
    private WiProProtocol() {
        super(null);
    } // end-ctor


    public WiProProtocol(IProtocolListener protocolListener) {
        super(protocolListener);
    }


    public byte getProtocolVersion() {
        return mProtocolVersion.getCurrentVersion();
    }

    /**
     * <b>This method is for the Test Cases only</b>
     *
     * @param version test protocol version
     */
    public void set_TEST_ProtocolMinVersion(byte version) {
        ProtocolConstants.PROTOCOL_VERSION_MIN = version;
        setProtocolVersion(version);
    }

    /**
     * <b>This method is for the Test Cases only</b>
     *
     * @param version test protocol version
     */
    public void set_TEST_ProtocolMaxVersion(byte version) {
        ProtocolConstants.PROTOCOL_VERSION_MAX = version;
    }

    @Override
    public void StartProtocolSession(byte sessionId) {

        Logger.i("Protocol session should start: " + sessionId);
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSession(ServiceType.RPC,
                sessionId, getProtocolVersion(), false);
        Logger.d(CLASS_NAME + " Start Protocol Session, protocol ver:" + getProtocolVersion());
        handleProtocolFrameToSend(header, null, 0, 0);
    }


    @Override
    protected void handleProtocolSessionStarted(ServiceType serviceType,
                                                byte sessionID, boolean encrypted, byte version, String correlationID) {
        super.handleProtocolSessionStarted(serviceType, sessionID, encrypted, version, correlationID);
        Logger.d(CLASS_NAME + " Protocol Session Started, protocol ver:" + version);
        setProtocolVersion(version);
    }


    public void StartProtocolService(ServiceType serviceType, Session session, boolean isCyphered) throws IllegalArgumentException {
        byte sessionId = session.getSessionId();
        Logger.i("Protocol service should start: " + serviceType);
        if (sessionId == 0) {
            throw new IllegalArgumentException("currentSession id 0 should be used to start " +
                    "currentSession only, provided id:" + sessionId + ", Service:" + serviceType);
        }

        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSession(serviceType, sessionId, getProtocolVersion(), isCyphered);
        handleProtocolFrameToSend(header, null, 0, 0);
    } // end-method


    private void sendStartProtocolSessionACK(ServiceType serviceType, byte sessionID) {
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSessionACK(serviceType,
                sessionID, 0x00, getProtocolVersion());
        handleProtocolFrameToSend(header, null, 0, 0);
    }

    public void SetHeartbeatSendInterval(int heartbeatSendInterval_ms) {
        _heartbeatSendInterval_ms = heartbeatSendInterval_ms;
    } // end-method

    public void SetHeartbeatReceiveInterval(int heartbeatReceiveInterval_ms) {
        _heartbeatReceiveInterval_ms = heartbeatReceiveInterval_ms;
    } // end-method

    public void EndProtocolService(ServiceType serviceType, byte sessionID) {
        byte[] data = BitConverter.intToByteArray(hashID);
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createEndSession(
                serviceType, sessionID, hashID, getProtocolVersion(), data.length);
        handleProtocolFrameToSend(header, data, 0, data.length);
    } // end-method

    public void SendMessage(ProtocolMessage protocolMessage, ServiceType serviceTypeToBeSecured) {
        protocolMessage.setRPCType(ProtocolMessage.RPCTYPE_REQUEST); //always sending a request
        final byte sessionID = protocolMessage.getSessionID();
        final byte protocolVersionToSend = getProtocolVersion();

        ProtocolMessageConverter protocolMessageConverter;
        if (serviceTypeToBeSecured == null) {
            protocolMessageConverter = new ProtocolMessageConverter(protocolMessage, getProtocolVersion()).generate();
        } else {
            protocolMessageConverter = new ProtocolMessageConverter(protocolMessage, protocolVersionToSend).generate(serviceTypeToBeSecured);
        }
        final byte[] data = protocolMessageConverter.getData();
        ServiceType serviceType = protocolMessageConverter.getServiceType();
        // Get the message lock for this protocol currentSession
        Object messageLock = _messageLocks.get(sessionID);
        if (messageLock == null) {
            handleProtocolError("Error sending protocol message to SYNC.",
                    new SyncException("Attempt to send protocol message prior to startSession ACK.",
                            SyncExceptionCause.SYNC_UNAVAILALBE)
            );
            return;
        }

        processFrameToSend(serviceType, sessionID, protocolMessage.isEncrypted(), data);
    }

    @Override
    public void SendMessage(final ProtocolMessage protocolMessage) {
        protocolMessage.setRPCType(ProtocolMessage.RPCTYPE_REQUEST); //always sending a request
        final byte sessionID = protocolMessage.getSessionID();
        final byte protocolVersionToSend = getProtocolVersion();

        ProtocolMessageConverter protocolMessageConverter = new ProtocolMessageConverter(
                protocolMessage, protocolVersionToSend).generate();
        final byte[] data = protocolMessageConverter.getData();
        final ServiceType serviceType = protocolMessageConverter.getServiceType();

        // Get the message lock for this protocol currentSession
        Object messageLock = _messageLocks.get(sessionID);
        if (messageLock == null) {
            handleProtocolError("Error sending protocol message to SYNC.",
                    new SyncException("Attempt to send protocol message prior to startSession ACK.",
                            SyncExceptionCause.SYNC_UNAVAILALBE)
            );
            return;
        }

        processProtocolMessage(serviceType, protocolVersionToSend, protocolMessage.isEncrypted(), data, sessionID);
    }

    private void sendFrameToTransport(ProtocolFrameHeader header) {
        handleProtocolFrameToSend(header, null, 0, 0);
    }

    @Override
    public void SendHeartBeatMessage(byte sessionId) {
        final ProtocolFrameHeader heartbeat =
                ProtocolFrameHeaderFactory.createHeartbeat(ServiceType.Heartbeat,
                        getProtocolVersion());
        final byte[] data = heartbeat.assembleHeaderBytes();
        processProtocolMessage(ServiceType.Heartbeat, getProtocolVersion(), false, data, sessionId);
    }

    @Override
    public void SendHeartBeatAckMessage(byte sessionId) {
        final ProtocolFrameHeader heartbeatAck =
                ProtocolFrameHeaderFactory.createHeartbeatACK(ServiceType.Heartbeat,
                        getProtocolVersion());
        final byte[] data = heartbeatAck.assembleHeaderBytes();
        processProtocolMessage(ServiceType.Heartbeat, getProtocolVersion(), false, data, sessionId);
    }


    private void processFrameToSend(ServiceType serviceType, byte sessionID, boolean encrypted, byte[] data) {

        // Get the message lock for this protocol currentSession
        Object messageLock = _messageLocks.get(sessionID);
        if (messageLock == null) {
            handleProtocolError("Error sending protocol message to SYNC.",
                    new SyncException("Attempt to send protocol message prior to startSession ACK.",
                            SyncExceptionCause.SYNC_UNAVAILALBE)
            );
            return;
        }

        processProtocolMessage(serviceType, getProtocolVersion(), encrypted, data, sessionID);
    }

    private void processProtocolMessage(ServiceType serviceType, byte protocolVersionToSend, final boolean encrypted,
                                        byte[] data, byte sessionID) {
        sendProtocolMessageProcessor.process(serviceType, protocolVersionToSend, data,
                MAX_DATA_SIZE, sessionID, getNextMessageId(),
                new SendProtocolMessageProcessor.ISendProtocolMessageProcessor() {

                    @Override
                    public void onProtocolFrameToSend(ProtocolFrameHeader header, byte[] data,
                                                      int offset, int length) {
                        header.setEncrypted(encrypted);
                        handleProtocolFrameToSend(header, data, offset, length);
                    }

                    @Override
                    public void onProtocolFrameToSendError(SendProtocolMessageProcessor.ERROR_TYPE errorType,
                                                           String message) {
                        switch (errorType) {
                            case DATA_NPE:
                                handleProtocolError("Error sending protocol message to SYNC.",
                                        new SyncException(message, SyncExceptionCause.INVALID_ARGUMENT)
                                );
                                break;
                        }
                    }
                }
        );
    }


    public void HandleReceivedBytes(byte[] receivedBytes, int receivedBytesLength) {
        int receivedBytesReadPos = 0;

        Logger.d(CLASS_NAME + " -> Bytes:" + BitConverter.bytesToHex(receivedBytes, 0, receivedBytesLength) +
                ", protocol ver:" + getProtocolVersion());

        Logger.d(CLASS_NAME + " -> protocol ver:" + getProtocolVersion() + " bytes length:" + receivedBytes.length);

        //Check for a version difference
        if (getProtocolVersion() == ProtocolConstants.PROTOCOL_VERSION_ONE) {
            byte parsedProtocolVersion = (byte) (receivedBytes[0] >>> 4);
            Logger.d(CLASS_NAME + " Parsed v:" + parsedProtocolVersion);

            //if (parsedProtocolVersion <= ProtocolConstants.PROTOCOL_VERSION_MAX) {
            setProtocolVersion(parsedProtocolVersion);
            //}

            //Nothing has been read into the buffer and version is 2
            if (parsedProtocolVersion >= ProtocolConstants.PROTOCOL_VERSION_TWO) {
                updateDataStructureToProtocolVersion(parsedProtocolVersion);
                //Buffer has something in it and version is 2
            } else if (parsedProtocolVersion >= ProtocolConstants.PROTOCOL_VERSION_TWO) {
                //safe current state of the buffer and also set the new version
                byte[] tempHeader = mHeaderBuf;
                updateDataStructureToProtocolVersion(parsedProtocolVersion);
                mHeaderBuf = tempHeader;
            }
        }

        // If I don't yet know the message size, grab those bytes.
        if (!mHaveHeader) {
            // If I can't get the size, just get the bytes that are there.
            int headerBytesNeeded = mHeaderBuf.length - mHeaderBufWritePos;
            if (receivedBytesLength < headerBytesNeeded) {
                System.arraycopy(receivedBytes, receivedBytesReadPos,
                        mHeaderBuf, mHeaderBufWritePos, receivedBytesLength);
                mHeaderBufWritePos += receivedBytesLength;
                return;
            } else {
                // If I got the size, allocate the buffer
                System.arraycopy(receivedBytes, receivedBytesReadPos,
                        mHeaderBuf, mHeaderBufWritePos, headerBytesNeeded);
                mHeaderBufWritePos += headerBytesNeeded;
                receivedBytesReadPos += headerBytesNeeded;
                mHaveHeader = true;
                mCurrentHeader = ProtocolFrameHeader.parseWiProHeader(mHeaderBuf);
                try {
                    mDataBuf = new byte[mCurrentHeader.getDataSize()];
                } catch (OutOfMemoryError e) {
                    // TODO - some terrible things is going on. _currentHeader.getDataSize()
                    // returns awfully big number during unregister - register cycle
                    Logger.e("No memory - no regrets.");

                }
                mDataBufWritePos = 0;
            }
        }

        int bytesLeft = receivedBytesLength - receivedBytesReadPos;
        int bytesNeeded = mDataBuf.length - mDataBufWritePos;

        // If I don't have enough bytes for the message, just grab what's there.

        if (bytesLeft < bytesNeeded) {
            System.arraycopy(receivedBytes, receivedBytesReadPos, mDataBuf,
                    mDataBufWritePos, bytesLeft);
            mDataBufWritePos += bytesLeft;
        } else {
            // Fill the buffer and call the handler!
            System.arraycopy(receivedBytes, receivedBytesReadPos, mDataBuf, mDataBufWritePos, bytesNeeded);
            receivedBytesReadPos += bytesNeeded;

            MessageFrameAssembler assembler = getFrameAssemblerForFrame(mCurrentHeader);
            handleProtocolFrameReceived(mCurrentHeader, mDataBuf, assembler);

            // Reset all class member variables for next frame
            mDataBuf = null;
            mDataBufWritePos = 0;
            mHaveHeader = false;
            mCurrentHeader = null;

            resetDataStructureToProtocolVersion();

            updateDataStructureToProtocolVersion(getProtocolVersion());

            // If there are any bytes left, recursive.
            int moreBytesLeft = receivedBytesLength - receivedBytesReadPos;
            if (moreBytesLeft > 0) {
                byte[] moreBytes = new byte[moreBytesLeft];
                System.arraycopy(receivedBytes, receivedBytesReadPos, moreBytes, 0, moreBytesLeft);
                HandleReceivedBytes(moreBytes, moreBytesLeft);
            }
        }
    }

    private MessageFrameAssembler getFrameAssemblerForFrame(ProtocolFrameHeader header) {
        Hashtable<Integer, MessageFrameAssembler> hashSessionID =
                ASSEMBLER_FOR_SESSION_ID.get(new Byte(header.getSessionID()));
        if (hashSessionID == null) {
            hashSessionID = new Hashtable<Integer, MessageFrameAssembler>();
            ASSEMBLER_FOR_SESSION_ID.put(new Byte(header.getSessionID()), hashSessionID);
        }

        MessageFrameAssembler ret = ASSEMBLER_FOR_MESSAGE_ID.get(new Integer(header.getMessageID()));
        if (ret == null) {
            ret = new MessageFrameAssembler();
            ASSEMBLER_FOR_MESSAGE_ID.put(new Integer(header.getMessageID()), ret);
        }

        return ret;
    }

    private int getNextMessageId() {
        return ++mMessageID;
    }

    protected class MessageFrameAssembler {
        protected boolean hasFirstFrame = false;
        protected boolean hasSecondFrame = false;
        protected ByteArrayOutputStream accumulator = null;
        protected int totalSize = 0;
        protected int framesRemaining = 0;

        protected void handleFirstDataFrame(ProtocolFrameHeader header, byte[] data) {
            //The message is new, so let's figure out how big it is.
            hasFirstFrame = true;
            totalSize = BitConverter.intFromByteArray(data, 0) - PROTOCOL_FRAME_HEADER_SIZE;
            framesRemaining = BitConverter.intFromByteArray(data, 4);
            accumulator = new ByteArrayOutputStream(totalSize);
        }

        protected void handleSecondFrame(ProtocolFrameHeader header, byte[] data) {
            handleRemainingFrame(header, data);
        }

        protected void handleRemainingFrame(ProtocolFrameHeader header, byte[] data) {
            accumulator.write(data, 0, header.getDataSize());
            notifyIfFinished(header);
        }

        protected void notifyIfFinished(final ProtocolFrameHeader header) {
            //if (framesRemaining == 0) {
            if (header.getFrameType() == FrameType.Consecutive && header.getFrameData() == 0x0) {

                final byte[] data = accumulator.toByteArray();
                createBigFrame(header, data);
                hasFirstFrame = false;
                hasSecondFrame = false;
                accumulator = null;
            } // end-if
        } // end-method

        private void createBigFrame(ProtocolFrameHeader header, byte[] data) {
            ProtocolMessage message = new ProtocolMessage();
            message.setServiceType(header.getServiceType());
            message.setSessionID(header.getSessionID());
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

            ASSEMBLER_FOR_MESSAGE_ID.remove(header.getMessageID());

            try {
                handleProtocolMessageReceived(message);
            } catch (Exception excp) {
                Logger.e(FAILURE_PROPAGATING_MSG + "onProtocolMessageReceived: " + excp.toString(), excp);
            } // end-catch

            hasFirstFrame = false;
            hasSecondFrame = false;
            accumulator = null;
        } // end-if


        protected void handleMultiFrameMessageFrame(ProtocolFrameHeader header, byte[] data) {
            //if (!hasFirstFrame) {
            //	hasFirstFrame = true;
            if (header.getFrameType() == FrameType.First) {
                handleFirstDataFrame(header, data);
            } else {
                if (getProtocolSecureManager() != null) {
                    try {
                        byte[] decipheredData = getProtocolSecureManager().sendDataToProxyServerByChunk(header.isEncrypted(), data);
                        handleRemainingFrame(header, decipheredData);
                    } catch (IOException e) {
                        Logger.i("Decipher error", e);
                    } catch (InterruptedException e) {
                        Logger.i("Decipher error", e);
                    }
                } else {
                    handleRemainingFrame(header, data);
                }
            }
        } // end-method


        protected void handleFrame(final ProtocolFrameHeader header, byte[] data) {
            processFrame(header, data);
        } // end-method


        private void processFrame(final ProtocolFrameHeader header, final byte[] data) {


            if (header.getFrameType().equals(FrameType.Control)) {
                handleControlFrame(header, data);
            } else {
                // Must be a form of data frame (single, first, consecutive, etc.)
                if (header.getFrameType() == FrameType.First
                        || header.getFrameType() == FrameType.Consecutive
                        ) {
                    handleMultiFrameMessageFrame(header, data);
                } else {
                    if (getProtocolSecureManager() != null) {
                        try {
                            byte[] decipheredData = getProtocolSecureManager().sendDataToProxyServerByteByByte(header.isEncrypted(), data);
                            handleSingleFrameMessageFrame(header, decipheredData);
                        } catch (IOException e) {
                            Logger.i("Decipher error", e);
                        } catch (InterruptedException e) {
                            Logger.i("Decipher error", e);
                        }

                    } else {
                        handleSingleFrameMessageFrame(header, data);
                    }
                }
            } // end-if
        }
    }

    private void handleProtocolHeartbeatACK(ProtocolFrameHeader header,
                                            byte[] data) {
        WiProProtocol.this.handleProtocolHeartbeatACK();
    } // end-method

    private void handleProtocolHeartbeat(ProtocolFrameHeader header,
                                         byte[] data) {
        WiProProtocol.this.handleProtocolHeartbeat();
    } // end-method

    private void handleControlFrame(ProtocolFrameHeader header, byte[] data) {
        if (header.getFrameData() == FrameDataControlFrameType.HeartbeatACK.getValue()) {
            handleProtocolHeartbeatACK(header, data);
        } else if (header.getFrameData() == FrameDataControlFrameType.Heartbeat.getValue()) {
            handleProtocolHeartbeat(header, data);
        } else if (header.getFrameData() == FrameDataControlFrameType.StartService.getValue()) {
            sendStartProtocolSessionACK(header.getServiceType(), header.getSessionID());
        } else if (header.getFrameData() == FrameDataControlFrameType.StartServiceACK.getValue()) {
            // Use this sessionID to create a message lock
            Object messageLock = _messageLocks.get(header.getSessionID());
            if (messageLock == null) {
                messageLock = new Object();
                _messageLocks.put(header.getSessionID(), messageLock);
            }
            //hashID = BitConverter.intFromByteArray(data, 0);
            if (getProtocolVersion() >= ProtocolConstants.PROTOCOL_VERSION_TWO) {
                hashID = header.getMessageID();
            }
            inspectStartServiceACKHeader(header);
        } else if (header.getFrameData() == FrameDataControlFrameType.StartServiceNACK.getValue()) {
            handleStartServiceNackFrame(header.getServiceType());
        } else if (header.getFrameData() == FrameDataControlFrameType.EndService.getValue()) {
            handleEndSessionFrame(header);
        } else if (header.getFrameData() == FrameDataControlFrameType.EndServiceNACK.getValue()) {
            //Logger.d(CLASS_NAME + " End Service NACK");
        } else if (header.getServiceType().getValue() == ServiceType.Mobile_Nav.getValue() &&
                header.getFrameData() == FrameDataControlFrameType.MobileNaviACK.getValue()) {
            handleMobileNavAckReceived(header);
        } else if (header.getFrameData() == FrameDataControlFrameType.EndServiceACK.getValue()) {
            handleEndSessionFrame(header);
        } else {
            Logger.w(CLASS_NAME + " Unknown frame data:" + header.getFrameData() + ", service type:" +
                    header.getServiceType());
        }
    } // end-method

    private void inspectStartServiceACKHeader(ProtocolFrameHeader header) {
        if (header.getServiceType().equals(ServiceType.RPC)) {
            if (!hasRPCStarted) {
                handleProtocolSessionStarted(header.getServiceType(),
                        header.getSessionID(), header.isEncrypted(), getProtocolVersion(), "");
                hasRPCStarted = true;
            } else {
                handleProtocolServiceStarted(header.getServiceType(),
                        header.getSessionID(), header.isEncrypted(), getProtocolVersion(), "");
            }
        } else {
            handleProtocolServiceStarted(header.getServiceType(),
                    header.getSessionID(), header.isEncrypted(), getProtocolVersion(), "");

        }
    }

    private void handleMobileNavAckReceived(ProtocolFrameHeader header) {
        _protocolListener.onMobileNavAckReceived(header.getMessageID());
    }

    private void handleSingleFrameMessageFrame(ProtocolFrameHeader header, byte[] data) {
        //Log.d(TAG, "SingleFrameMessageFrame:" + header.getDataSize() + ", data size:" + data.length);
        ProtocolMessage message = new ProtocolMessage();
        if (header.getServiceType() == ServiceType.RPC) {
            message.setMessageType(MessageType.RPC);
        } else if (header.getServiceType() == ServiceType.Bulk_Data) {
            message.setMessageType(MessageType.BULK);
        } // end-if
        message.setServiceType(header.getServiceType());
        message.setSessionID(header.getSessionID());
        message.setEncrypted(header.isEncrypted());
        //If it is WiPro 2.0 it must have binary header

        if (getProtocolVersion() >= ProtocolConstants.PROTOCOL_VERSION_TWO) {

            message.setVersion(getProtocolVersion());
            // Set Secure Service payload data
            if (message.getServiceType().equals(ServiceType.Heartbeat)) {
                message.setData(data);
            } else {
                BinaryFrameHeader binFrameHeader = BinaryFrameHeader.parseBinaryHeader(data);
                message.setRPCType(binFrameHeader.getRPCType());
                message.setFunctionID(binFrameHeader.getFunctionID());
                message.setCorrID(binFrameHeader.getCorrID());
                if (binFrameHeader.getJsonSize() > 0) {
                    message.setData(binFrameHeader.getJsonData());
                }
                if (binFrameHeader.getBulkData() != null) {
                    message.setBulkData(binFrameHeader.getBulkData());
                }
            }

        } else {
            message.setData(data);
        }


        ASSEMBLER_FOR_MESSAGE_ID.remove(header.getMessageID());

        if (isAppUnregistered(message)) {
            Logger.i("App is unregistered");
            handleAppUnregistered();
        }

        try {
            handleProtocolMessageReceived(message);
        } catch (Exception ex) {
            Logger.e(FAILURE_PROPAGATING_MSG + "onProtocolMessageReceived: " + ex.toString(), ex);
            handleProtocolError(FAILURE_PROPAGATING_MSG + "onProtocolMessageReceived: ", ex);
        } // end-catch
    } // end-method

    private boolean isAppUnregistered(ProtocolMessage message) {
        return (message.getRPCType() == ProtocolMessage.RPCTYPE_RESPONSE) &&
                (message.getFunctionID() == FunctionID
                        .getFunctionID(Names.UnregisterAppInterface));
    }


    private void handleEndSessionFrame(ProtocolFrameHeader header) {
        Logger.d(CLASS_NAME + " EndServiceACKHeader v:" + header.getVersion());
        if (getProtocolVersion() >= ProtocolConstants.PROTOCOL_VERSION_TWO) {
            if (hashID == header.getMessageID()) {
                handleProtocolServiceEnded(header.getServiceType(), header.getSessionID(), "");
            }
        } else {
            handleProtocolServiceEnded(header.getServiceType(), header.getSessionID(), "");
        }
    }

    private void handleStartServiceNackFrame(ServiceType serviceType) {
        _protocolListener.onStartServiceNackReceived(serviceType);
    }
}