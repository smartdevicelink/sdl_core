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

    private static final String CLASS_NAME = WiProProtocol.class.getSimpleName();

    public static final int MTU_SIZE = 1500;
    private final static String FailurePropagating_Msg = "Failure propagating ";
    public static int PROTOCOL_FRAME_HEADER_SIZE = ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_DEFAULT;
    public static final int SSL_OVERHEAD = 64;
    public static int MAX_DATA_SIZE = MTU_SIZE - PROTOCOL_FRAME_HEADER_SIZE - SSL_OVERHEAD;
    private ProtocolVersion mProtocolVersion = new ProtocolVersion();

    boolean _haveHeader = false;
    byte[] _headerBuf = new byte[PROTOCOL_FRAME_HEADER_SIZE];
    int _headerBufWritePos = 0;
    ProtocolFrameHeader _currentHeader = null;
    byte[] _dataBuf = null;
    int _dataBufWritePos = 0;
    int hashID = 0;
    int messageID = 0;
    Hashtable<Integer, MessageFrameAssembler> _assemblerForMessageID = new Hashtable<Integer, MessageFrameAssembler>();
    Hashtable<Byte, Hashtable<Integer, MessageFrameAssembler>> _assemblerForSessionID = new Hashtable<Byte, Hashtable<Integer, MessageFrameAssembler>>();
    protected Hashtable<Byte, Object> _messageLocks = new Hashtable<Byte, Object>();
    // NOTE: To date, not implemented on SYNC
    private int _heartbeatSendInterval_ms = 0;
    // NOTE: To date, not implemented on SYNC
    private int _heartbeatReceiveInterval_ms = 0;


    // Hide no-arg ctor
    private WiProProtocol() {
        super(null);
    } // end-ctor


    public WiProProtocol(IProtocolListener protocolListener) {
        super(protocolListener);
        setProtocolVersion(ProtocolConstants.PROTOCOL_VERSION_MIN);
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
     * @param version test protocol version
     */
    public void set_TEST_ProtocolMaxVersion(byte version) {
        ProtocolConstants.PROTOCOL_VERSION_MAX = version;
    }

    public void setProtocolVersion(byte version) {
        mProtocolVersion.setCurrentVersion(version);

        if (mProtocolVersion.getCurrentVersion() >= ProtocolConstants.PROTOCOL_VERSION_TWO) {

            // TODO : Incorporate SSL overhead const
            // Implement here

            PROTOCOL_FRAME_HEADER_SIZE = ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2;
            MAX_DATA_SIZE = MTU_SIZE - PROTOCOL_FRAME_HEADER_SIZE - SSL_OVERHEAD;
            _headerBuf = new byte[PROTOCOL_FRAME_HEADER_SIZE];
            updateDataStructureToProtocolVersion(version);
        } else {
            Logger.d(CLASS_NAME + " Protocol version:" + mProtocolVersion.getCurrentVersion());
        }
    }


    @Override
    public void StartProtocolSession(byte sessionId) {

        Logger.i("Protocol session should start: " + sessionId);
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSession(ServiceType.RPC,
                sessionId, getProtocolVersion(), false);
        Logger.d(CLASS_NAME + " Start Protocol Session, protocol ver:" +

                        getProtocolVersion()
        );
        sendFrameToTransport(header);
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

        sendFrameToTransport(header);
    } // end-method

    private void sendStartProtocolSessionACK(ServiceType serviceType, byte sessionID) {
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSessionACK(serviceType,
                sessionID, 0x00, getProtocolVersion());
        sendFrameToTransport(header);
    } // end-method

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

    /**
     * @param protocolMsg
     * @param serviceTypeToBeSecured
     */
    public void SendMessage(ProtocolMessage protocolMsg, ServiceType serviceTypeToBeSecured) {
        protocolMsg.setRPCType((byte) 0x00); //always sending a request
        final byte sessionID = protocolMsg.getSessionID();

        byte protocolVersionToSend = getProtocolVersion();
        /*if (protocolVersionToSend > ProtocolConstants.PROTOCOL_VERSION_MAX) {
            protocolVersionToSend = ProtocolConstants.PROTOCOL_VERSION_MAX;
        }*/

        ProtocolMessageConverter protocolMessageConverter;
        if (serviceTypeToBeSecured == null) {
            protocolMessageConverter = new ProtocolMessageConverter(protocolMsg, getProtocolVersion()).generate();
        } else {
            protocolMessageConverter = new ProtocolMessageConverter(protocolMsg, protocolVersionToSend).generate(serviceTypeToBeSecured);
        }
        final byte[] data = protocolMessageConverter.getData();
        ServiceType serviceType = protocolMessageConverter.getSessionType();

        processFrameToSend(serviceType, sessionID, protocolMsg.isEncrypted(), data);
    }

    private void processFrameToSend(ServiceType serviceType, byte sessionID, boolean encrypted, byte[] data) {
        // Get the message lock for this protocol currentSession
        Object messageLock = _messageLocks.get(sessionID);
        if (messageLock == null) {
            handleProtocolError("Error sending protocol message to SYNC.",
                    new SyncException("Attempt to send protocol message prior to startSession ACK.",
                            SyncExceptionCause.SYNC_UNAVAILALBE));
            return;
        }

        synchronized (messageLock) {
            if (data.length > MAX_DATA_SIZE) {

                messageID++;

                ProtocolFrameHeader firstHeader = ProtocolFrameHeaderFactory.createMultiSendDataFirst(serviceType, sessionID, messageID, getProtocolVersion());
                firstHeader.setEncrypted(false);

                // Assemble first frame.
                int frameCount = data.length / MAX_DATA_SIZE;
                if (data.length % MAX_DATA_SIZE > 0) {
                    frameCount++;
                }
                //byte[] firstFrameData = new byte[PROTOCOL_FRAME_HEADER_SIZE];
                byte[] firstFrameData = new byte[8];
                // First four bytes are data size.
                System.arraycopy(BitConverter.intToByteArray(data.length), 0, firstFrameData, 0, 4);
                // Second four bytes are frame count.
                System.arraycopy(BitConverter.intToByteArray(frameCount), 0, firstFrameData, 4, 4);

                handleProtocolFrameToSend(firstHeader, firstFrameData, 0, firstFrameData.length);

                int currentOffset = 0;
                byte frameSequenceNumber = 0;

                for (int i = 0; i < frameCount; i++) {
                    if (i < (frameCount - 1)) {
                        ++frameSequenceNumber;
                        if (frameSequenceNumber ==
                                ProtocolFrameHeader.FrameDataFinalConsecutiveFrame) {
                            // we can't use 0x00 as frameSequenceNumber, because
                            // it's reserved for the last frame
                            ++frameSequenceNumber;
                        }
                    } else {
                        frameSequenceNumber = ProtocolFrameHeader.FrameDataFinalConsecutiveFrame;
                    } // end-if

                    int bytesToWrite = data.length - currentOffset;
                    if (bytesToWrite > MAX_DATA_SIZE) {
                        bytesToWrite = MAX_DATA_SIZE;
                    }


                    ProtocolFrameHeader consecHeader = ProtocolFrameHeaderFactory.createMultiSendDataRest(serviceType, sessionID, bytesToWrite, frameSequenceNumber, messageID, getProtocolVersion());
                    consecHeader.setEncrypted(encrypted);

                    handleProtocolFrameToSend(consecHeader, data, currentOffset, bytesToWrite);
                    currentOffset += bytesToWrite;
                }
            } else {
                messageID++;

                ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createSingleSendData(serviceType, sessionID, data.length, messageID, getProtocolVersion());
                header.setEncrypted(encrypted);

                handleProtocolFrameToSend(header, data, 0, data.length);
            }
        }
    }

    public void SendMessage(ProtocolMessage protocolMsg) {
        SendMessage(protocolMsg, null);
    }

    private void sendFrameToTransport(ProtocolFrameHeader header) {
        handleProtocolFrameToSend(header, null, 0, 0);
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

            if (parsedProtocolVersion <= ProtocolConstants.PROTOCOL_VERSION_MAX) {
                setProtocolVersion(parsedProtocolVersion);
            }

            //Nothing has been read into the buffer and version is 2
            if (parsedProtocolVersion >= ProtocolConstants.PROTOCOL_VERSION_TWO) {
                updateDataStructureToProtocolVersion(parsedProtocolVersion);
                //Buffer has something in it and version is 2
            } else if (parsedProtocolVersion >= ProtocolConstants.PROTOCOL_VERSION_TWO) {
                //safe current state of the buffer and also set the new version
                byte[] tempHeader = _headerBuf;
                updateDataStructureToProtocolVersion(parsedProtocolVersion);
                _headerBuf = tempHeader;
            }
        }

        // If I don't yet know the message size, grab those bytes.
        if (!_haveHeader) {
            // If I can't get the size, just get the bytes that are there.
            int headerBytesNeeded = _headerBuf.length - _headerBufWritePos;
            if (receivedBytesLength < headerBytesNeeded) {
                System.arraycopy(receivedBytes, receivedBytesReadPos,
                        _headerBuf, _headerBufWritePos, receivedBytesLength);
                _headerBufWritePos += receivedBytesLength;
                return;
            } else {
                // If I got the size, allocate the buffer
                System.arraycopy(receivedBytes, receivedBytesReadPos,
                        _headerBuf, _headerBufWritePos, headerBytesNeeded);
                _headerBufWritePos += headerBytesNeeded;
                receivedBytesReadPos += headerBytesNeeded;
                _haveHeader = true;
                _currentHeader = ProtocolFrameHeader.parseWiProHeader(_headerBuf);
                try {
                    _dataBuf = new byte[_currentHeader.getDataSize()];
                } catch (OutOfMemoryError e) {

                    // TODO - some terrible things is going on. _currentHeader.getDataSize()
                    // returns awfully big number during unregister - register cycle
                    Logger.e("No memory - no regrets.");

                }
                _dataBufWritePos = 0;
            }
        }

        int bytesLeft = receivedBytesLength - receivedBytesReadPos;
        int bytesNeeded = _dataBuf.length - _dataBufWritePos;

        // If I don't have enough bytes for the message, just grab what's there.

        if (bytesLeft < bytesNeeded) {
            System.arraycopy(receivedBytes, receivedBytesReadPos, _dataBuf,
                    _dataBufWritePos, bytesLeft);
            _dataBufWritePos += bytesLeft;
        } else {
            // Fill the buffer and call the handler!
            System.arraycopy(receivedBytes, receivedBytesReadPos, _dataBuf, _dataBufWritePos, bytesNeeded);
            receivedBytesReadPos += bytesNeeded;

            MessageFrameAssembler assembler = getFrameAssemblerForFrame(_currentHeader);
            handleProtocolFrameReceived(_currentHeader, _dataBuf, assembler);

            // Reset all class member variables for next frame
            _dataBuf = null;
            _dataBufWritePos = 0;
            _haveHeader = false;
            _headerBuf = new byte[PROTOCOL_FRAME_HEADER_SIZE];
            _currentHeader = null;
            _headerBufWritePos = 0;

            updateDataStructureToProtocolVersion(getProtocolVersion());

            // If there are any bytes left, recursive.
            int moreBytesLeft = receivedBytesLength - receivedBytesReadPos;
            if (moreBytesLeft > 0) {
                byte[] moreBytes = new byte[moreBytesLeft];
                System.arraycopy(receivedBytes, receivedBytesReadPos,
                        moreBytes, 0, moreBytesLeft);
                HandleReceivedBytes(moreBytes, moreBytesLeft);
            }
        }
    }

    protected MessageFrameAssembler getFrameAssemblerForFrame(ProtocolFrameHeader header) {
        Hashtable<Integer, MessageFrameAssembler> hashSessionID = _assemblerForSessionID.get(new Byte(header.getSessionID()));
        if (hashSessionID == null) {
            hashSessionID = new Hashtable<Integer, MessageFrameAssembler>();
            _assemblerForSessionID.put(new Byte(header.getSessionID()), hashSessionID);
        } // end-if

        MessageFrameAssembler ret = _assemblerForMessageID.get(new Integer(header.getMessageID()));
        if (ret == null) {
            ret = new MessageFrameAssembler();
            _assemblerForMessageID.put(new Integer(header.getMessageID()), ret);
        } // end-if

        return ret;
    } // end-method

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
            message.setSessionType(header.getServiceType());
            message.setSessionID(header.getSessionID());
            //If it is WiPro 2.0 it must have binary header
            if (header.getVersion() >= 2) {
                message.setVersion(header.getVersion());
                if (message.getServiceType().equals(ServiceType.Heartbeat)) {
                    message.setData(data);
                } else {
                    //If it is WiPro 2.0 it must have binary header
                    if (getProtocolVersion() >= ProtocolConstants.PROTOCOL_VERSION_TWO) {
                        BinaryFrameHeader binFrameHeader = BinaryFrameHeader.
                                parseBinaryHeader(data);

                        message.setRPCType(binFrameHeader.getRPCType());
                        message.setFunctionID(binFrameHeader.getFunctionID());
                        message.setCorrID(binFrameHeader.getCorrID());
                        if (binFrameHeader.getJsonSize() > 0)
                            message.setData(binFrameHeader.getJsonData());
                        if (binFrameHeader.getBulkData() != null)
                            message.setBulkData(binFrameHeader.getBulkData());
                    } else message.setData(data);


                    _assemblerForMessageID.remove(header.getMessageID());

                    try {
                        handleProtocolMessageReceived(message);
                    } catch (Exception excp) {
                        Logger.e(FailurePropagating_Msg + "onProtocolMessageReceived: " + excp.toString(), excp);
                    } // end-catch

                }
            }
        }

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
                        Logger.i( "Decipher error", e);
                    } catch (InterruptedException e) {
                        Logger.i( "Decipher error", e);
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
                            Logger.i( "Decipher error", e);
                        } catch (InterruptedException e) {
                            Logger.i( "Decipher error", e);
                        }

                    } else {
                        handleSingleFrameMessageFrame(header, data);
                    }
                }
            } // end-if
        }


        private void handleProtocolHeartbeatACK(ProtocolFrameHeader header,
                                                byte[] data) {
            WiProProtocol.this.handleProtocolHeartbeatACK();
        } // end-method

        private void handleControlFrame(ProtocolFrameHeader header, byte[] data) {
            if (header.getFrameData() == FrameDataControlFrameType.HeartbeatACK.getValue()) {
                handleProtocolHeartbeatACK(header, data);
                // TODO heartbeat messages currently are not handled
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
            } else if (header.getServiceType().getValue() == ServiceType.Mobile_Nav.getValue() && header.getFrameData() == FrameDataControlFrameType.MobileNaviACK.getValue()) {
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
            message.setSessionType(header.getServiceType());
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


            _assemblerForMessageID.remove(header.getMessageID());

            if (isAppUnregistered(message)) {
                Logger.i("App is unregistered");
                handleAppUnregistered();
            }

            try {
                handleProtocolMessageReceived(message);
            } catch (Exception ex) {
                Logger.e(FailurePropagating_Msg + "onProtocolMessageReceived: " + ex.toString(), ex);
                handleProtocolError(FailurePropagating_Msg + "onProtocolMessageReceived: ", ex);
            } // end-catch
        } // end-method

        private boolean isAppUnregistered(ProtocolMessage message) {
            return (message.getRPCType() == ProtocolMessage.RPCTYPE_RESPONSE) &&
                    (message.getFunctionID() == FunctionID
                            .getFunctionID(Names.UnregisterAppInterface));
        }

    } // end-class

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

    private void updateDataStructureToProtocolVersion(byte version) {
        Logger.d(CLASS_NAME + " Data structure updated to v:" + version);
        // TODO : Incorporate SSL overhead const
        // Implement here

        switch (version) {
            case ProtocolConstants.PROTOCOL_VERSION_ONE:
                PROTOCOL_FRAME_HEADER_SIZE = ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_1;
                break;
            default:
                PROTOCOL_FRAME_HEADER_SIZE = ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2;
                break;
        }

        MAX_DATA_SIZE = MTU_SIZE - PROTOCOL_FRAME_HEADER_SIZE;
        _headerBuf = new byte[PROTOCOL_FRAME_HEADER_SIZE];
    }
}
