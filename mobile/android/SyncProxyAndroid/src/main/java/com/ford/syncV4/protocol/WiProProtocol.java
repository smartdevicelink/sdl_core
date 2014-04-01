package com.ford.syncV4.protocol;

import android.util.Log;

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
import com.ford.syncV4.util.DebugTool;

import java.io.ByteArrayOutputStream;
import java.util.Hashtable;

public class WiProProtocol extends AbstractProtocol {

    private static final String TAG = "WiProProtocol";

    public static final int MTU_SIZE = 1500;
    private final static String FailurePropagating_Msg = "Failure propagating ";
    public static int HEADER_SIZE = ProtocolConstants.HEADER_SIZE_DEFAULT;
    public static int MAX_DATA_SIZE = MTU_SIZE - HEADER_SIZE;
    byte mProtocolVersion = ProtocolConstants.PROTOCOL_VERSION_DEFAULT;
    boolean _haveHeader = false;
    byte[] _headerBuf = new byte[HEADER_SIZE];
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
    } // end-ctor

    public byte getProtocolVersion() {
        return mProtocolVersion;
    }

    public void setProtocolVersion(byte version) {
        mProtocolVersion = version;
        if (mProtocolVersion == 2) {
            HEADER_SIZE = ProtocolConstants.HEADER_SIZE_V_2;
            MAX_DATA_SIZE = MTU_SIZE - HEADER_SIZE;
            _headerBuf = new byte[HEADER_SIZE];
        }
    }

    public void StartProtocolSession(byte sessionId) {
        DebugTool.logInfo("Protocol session should start: " + sessionId);
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSession(ServiceType.RPC,
                sessionId, getProtocolVersion());
        Log.d(TAG, "Start Protocol Session, protocol ver:" + getProtocolVersion());
        sendFrameToTransport(header);
    }

    @Override
    public void handleProtocolSessionStarted(ServiceType serviceType, byte sessionID, byte version,
                                             String correlationID) {
        super.handleProtocolSessionStarted(serviceType, sessionID, version, correlationID);

        Log.d(TAG, "Protocol Session Started, protocol ver:" + version);
        setProtocolVersion(version);
    }

    public void StartProtocolService(ServiceType serviceType, Session session) throws IllegalArgumentException {
        byte sessionId = session.getSessionId();
        DebugTool.logInfo("Protocol service should start: " + serviceType);
        if (sessionId == 0) {
            throw new IllegalArgumentException("currentSession id 0 should be used to start " +
                    "currentSession only, provided id:" + sessionId + ", Service:" + serviceType);
        }
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSession(serviceType,
                sessionId, getProtocolVersion());
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

    public void SendMessage(ProtocolMessage protocolMsg) {
        protocolMsg.setRPCType((byte) 0x00); //always sending a request
        ServiceType serviceType = protocolMsg.getServiceType();
        byte sessionID = protocolMsg.getSessionID();

        ProtocolMessageConverter protocolMessageConverter = new ProtocolMessageConverter(
                protocolMsg, getProtocolVersion()).generate();
        byte[] data = protocolMessageConverter.getData();
        serviceType = protocolMessageConverter.getSessionType();


        // Get the message lock for this protocol currentSession
        Object messageLock = _messageLocks.get(sessionID);
        if (messageLock == null) {
            handleProtocolError("Error sending protocol message to SYNC.",
                    new SyncException("Attempt to send protocol message prior to startSession ACK.", SyncExceptionCause.SYNC_UNAVAILALBE));
            return;
        }

        synchronized (messageLock) {
            if (data.length > MAX_DATA_SIZE) {

                messageID++;
                ProtocolFrameHeader firstHeader = ProtocolFrameHeaderFactory.createMultiSendDataFirst(serviceType, sessionID, messageID, getProtocolVersion());

                // Assemble first frame.
                int frameCount = data.length / MAX_DATA_SIZE;
                if (data.length % MAX_DATA_SIZE > 0) {
                    frameCount++;
                }
                //byte[] firstFrameData = new byte[HEADER_SIZE];
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
                    handleProtocolFrameToSend(consecHeader, data, currentOffset, bytesToWrite);
                    currentOffset += bytesToWrite;
                }
            } else {
                messageID++;
                ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createSingleSendData(serviceType, sessionID, data.length, messageID, getProtocolVersion());
                handleProtocolFrameToSend(header, data, 0, data.length);

            }
        }
    }

    private void sendFrameToTransport(ProtocolFrameHeader header) {
        handleProtocolFrameToSend(header, null, 0, 0);
    }

    public void HandleReceivedBytes(byte[] receivedBytes, int receivedBytesLength) {
        int receivedBytesReadPos = 0;

        Log.d(TAG, "-> Bytes:" + BitConverter.bytesToHex(receivedBytes, 0, receivedBytesLength) +
                ", protocol ver:" + getProtocolVersion());

        //Check for a version difference
        if (getProtocolVersion() == 1) {
            //Nothing has been read into the buffer and version is 2
            if (_headerBufWritePos == 0 && (byte) (receivedBytes[0] >>> 4) == 2) {
                setProtocolVersion((byte) (receivedBytes[0] >>> 4));
                //Buffer has something in it and version is 2
            } else if ((byte) (_headerBuf[0] >>> 4) == 2) {
                //safe current state of the buffer and also set the new version
                byte[] tempHeader = new byte[_headerBufWritePos];
                tempHeader = _headerBuf;
                setProtocolVersion((byte) (_headerBuf[0] >>> 4));
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
                Log.d(TAG, "HeaderBuf:" + _headerBuf.length + ", header pos:" + _headerBufWritePos +
                    ", bytes needed:" + headerBytesNeeded);
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
                    DebugTool.logError(e.toString() + " No memory - no regrets.");
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
            return;
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
            _headerBuf = new byte[HEADER_SIZE];
            _currentHeader = null;
            _headerBufWritePos = 0;

            // If there are any bytes left, recurse.
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

        MessageFrameAssembler ret = (MessageFrameAssembler) _assemblerForMessageID.get(new Integer(header.getMessageID()));
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
            totalSize = BitConverter.intFromByteArray(data, 0) - HEADER_SIZE;
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

        protected void notifyIfFinished(ProtocolFrameHeader header) {
            //if (framesRemaining == 0) {
            if (header.getFrameType() == FrameType.Consecutive && header.getFrameData() == 0x0) {
                ProtocolMessage message = new ProtocolMessage();
                message.setSessionType(header.getServiceType());
                message.setSessionID(header.getSessionID());
                //If it is WiPro 2.0 it must have binary header
                if (getProtocolVersion() == 2) {
                    BinaryFrameHeader binFrameHeader = BinaryFrameHeader.
                            parseBinaryHeader(accumulator.toByteArray());
                    message.setVersion(getProtocolVersion());
                    message.setRPCType(binFrameHeader.getRPCType());
                    message.setFunctionID(binFrameHeader.getFunctionID());
                    message.setCorrID(binFrameHeader.getCorrID());
                    if (binFrameHeader.getJsonSize() > 0)
                        message.setData(binFrameHeader.getJsonData());
                    if (binFrameHeader.getBulkData() != null)
                        message.setBulkData(binFrameHeader.getBulkData());
                } else message.setData(accumulator.toByteArray());

                _assemblerForMessageID.remove(header.getMessageID());

                try {
                    handleProtocolMessageReceived(message);
                } catch (Exception excp) {
                    DebugTool.logError(FailurePropagating_Msg + "onProtocolMessageReceived: " + excp.toString(), excp);
                } // end-catch

                hasFirstFrame = false;
                hasSecondFrame = false;
                accumulator = null;
            } // end-if
        } // end-method

        protected void handleMultiFrameMessageFrame(ProtocolFrameHeader header, byte[] data) {
            //if (!hasFirstFrame) {
            //	hasFirstFrame = true;
            if (header.getFrameType() == FrameType.First) {
                handleFirstDataFrame(header, data);
            }

            //} else if (!hasSecondFrame) {
            //	hasSecondFrame = true;
            //	framesRemaining--;
            //	handleSecondFrame(header, data);
            //} else {
            //	framesRemaining--;
            else {
                handleRemainingFrame(header, data);
            }

            //}
        } // end-method

        protected void handleFrame(ProtocolFrameHeader header, byte[] data) {
            Log.d(TAG, "Handle frame, type:" + header.getFrameType());
            if (header.getFrameType().equals(FrameType.Control)) {
                handleControlFrame(header, data);
            } else {
                // Must be a form of data frame (single, first, consecutive, etc.)
                if (header.getFrameType() == FrameType.First
                        || header.getFrameType() == FrameType.Consecutive
                        ) {
                    handleMultiFrameMessageFrame(header, data);
                } else {
                    handleSingleFrameMessageFrame(header, data);
                }
            } // end-if
        } // end-method

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
                if (getProtocolVersion() == 2) {
                    hashID = header.getMessageID();
                }
                inspectStartServiceACKHeader(header);
            } else if (header.getFrameData() == FrameDataControlFrameType.StartServiceNACK.getValue()) {
                handleStartServiceNackFrame(header.getServiceType());
            } else if (header.getFrameData() == FrameDataControlFrameType.EndService.getValue()) {
                handleEndSessionFrame(header);
            } else if (header.getFrameData() == FrameDataControlFrameType.EndServiceNACK.getValue()) {
                //Log.d(TAG, "End Service NACK");
            } else if (header.getServiceType().getValue() == ServiceType.Mobile_Nav.getValue() && header.getFrameData() == FrameDataControlFrameType.MobileNaviACK.getValue()) {
                handleMobileNavAckReceived(header);
            } else if (header.getFrameData() == FrameDataControlFrameType.EndServiceACK.getValue()) {
                handleEndSessionFrame(header);
            } else {
                Log.w(TAG, "Unknown frame data:" + header.getFrameData() + ", service type:" +
                        header.getServiceType());
            }
        } // end-method

        private void inspectStartServiceACKHeader(ProtocolFrameHeader header) {
            if (header.getServiceType().equals(ServiceType.RPC)) {
                handleProtocolSessionStarted(header.getServiceType(),
                        header.getSessionID(), getProtocolVersion(), "");
            } else {
                handleProtocolServiceStarted(header.getServiceType(),
                        header.getSessionID(), getProtocolVersion(), "");
            }
        }

        private void handleMobileNavAckReceived(ProtocolFrameHeader header) {
            _protocolListener.onMobileNavAckReceived(header.getMessageID());
        }

        private void handleSingleFrameMessageFrame(ProtocolFrameHeader header, byte[] data) {
            ProtocolMessage message = new ProtocolMessage();
            if (header.getServiceType() == ServiceType.RPC) {
                message.setMessageType(MessageType.RPC);
            } else if (header.getServiceType() == ServiceType.Bulk_Data) {
                message.setMessageType(MessageType.BULK);
            } // end-if
            message.setSessionType(header.getServiceType());
            message.setSessionID(header.getSessionID());
            //If it is WiPro 2.0 it must have binary header
            if (getProtocolVersion() == 2) {
                BinaryFrameHeader binFrameHeader = BinaryFrameHeader.
                        parseBinaryHeader(data);
                message.setVersion(getProtocolVersion());
                message.setRPCType(binFrameHeader.getRPCType());
                message.setFunctionID(binFrameHeader.getFunctionID());
                message.setCorrID(binFrameHeader.getCorrID());
                if (binFrameHeader.getJsonSize() > 0) message.setData(binFrameHeader.getJsonData());
                if (binFrameHeader.getBulkData() != null)
                    message.setBulkData(binFrameHeader.getBulkData());
            } else message.setData(data);

            _assemblerForMessageID.remove(header.getMessageID());

            if (isAppUnregistered(message)) {
                DebugTool.logInfo("App is unregistered");
                handleAppUnregistered();
            }

            try {
                handleProtocolMessageReceived(message);
            } catch (Exception ex) {
                DebugTool.logError(FailurePropagating_Msg + "onProtocolMessageReceived: " + ex.toString(), ex);
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
        if (getProtocolVersion() == 2) {
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