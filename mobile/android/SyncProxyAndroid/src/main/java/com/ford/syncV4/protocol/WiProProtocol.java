package com.ford.syncV4.protocol;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.util.BitConverter;
import com.ford.syncV4.util.logger.Logger;

import java.util.Hashtable;

public class WiProProtocol extends AbstractProtocol {

    protected Hashtable<Byte, Object> _messageLocks = new Hashtable<Byte, Object>();

    private static final String CLASS_NAME = WiProProtocol.class.getSimpleName();
    private final Hashtable<Integer, MessageFrameAssembler> ASSEMBLER_FOR_MESSAGE_ID =
            new Hashtable<Integer, MessageFrameAssembler>();
    private final Hashtable<Byte, Hashtable<Integer, MessageFrameAssembler>> ASSEMBLER_FOR_SESSION_ID =
            new Hashtable<Byte, Hashtable<Integer, MessageFrameAssembler>>();

    private final SendProtocolMessageProcessor sendProtocolMessageProcessor =
            new SendProtocolMessageProcessor();

    private final Hashtable<Byte, Integer> sessionsHashIds = new Hashtable<Byte, Integer>();

    private ProtocolFrameHeader mCurrentHeader = null;
    private boolean mHaveHeader = false;
    private byte[] mDataBuf = null;
    private int mDataBufWritePos = 0;
    private int mMessageID = 0;

    public WiProProtocol(IProtocolListener protocolListener) {
        super(protocolListener);

        sendProtocolMessageProcessor.setCallback(new SendProtocolMessageProcessor.ISendProtocolMessageProcessor() {
            @Override
            public void onProtocolFrameToSend(ProtocolFrameHeader header, byte[] data, int offset,
                                              int length) {
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
        });
    }

    public void shutDownAudioStreamExecutor() throws InterruptedException {
        sendProtocolMessageProcessor.shutdownAudioExecutors();
    }

    public void shutDownMobileNaviStreamExecutor() throws InterruptedException {
        sendProtocolMessageProcessor.shutdownNaviExecutors();
    }

    @Override
    public void StartProtocolSession(byte sessionId) {
        sendProtocolMessageProcessor.processStartSession(getProtocolVersion(), sessionId);
    }

    @Override
    public void handleProtocolSessionStarted(ServiceType serviceType, byte sessionId, byte version) {
        super.handleProtocolSessionStarted(serviceType, sessionId, version);

        Logger.d(CLASS_NAME + " Protocol Session Started, protocol ver:" + version);
        setProtocolVersion(version);
    }

    @Override
    public void StartProtocolService(ServiceType serviceType, byte sessionId)
            throws IllegalArgumentException {
        if (sessionId == Session.DEFAULT_SESSION_ID) {
            throw new IllegalArgumentException("syncSession id 0 should be used to start " +
                    "syncSession only, provided id:" + sessionId + ", Service:" + serviceType);
        }
        sendProtocolMessageProcessor.processStartService(serviceType, getProtocolVersion(),
                sessionId);
    }

    @Override
    public void EndProtocolService(ServiceType serviceType, byte sessionId) {
        int hasId = getHashIdBySessionId(sessionId);
        Logger.d("End Service hashId:" + hasId + " sesId:" + sessionId);
        sendProtocolMessageProcessor.processEndService(serviceType, hasId,
                getProtocolVersion(), sessionId);
    }

    @Override
    public void SendHeartBeatMessage(byte sessionId) {
        sendProtocolMessageProcessor.processHeartbeat(getProtocolVersion(), sessionId);
    }

    @Override
    public void SendHeartBeatAckMessage(byte sessionId) {
        Logger.d(CLASS_NAME + " SendHeartBeatAckMessage");
        sendProtocolMessageProcessor.processHeartbeatAck(getProtocolVersion(), sessionId);
    }

    @Override
    public void SendMessage(ProtocolMessage protocolMessage) {
        protocolMessage.setRPCType(ProtocolMessage.RPCTYPE_REQUEST); //always sending a request
        final byte sessionId = protocolMessage.getSessionID();

        // Get the message lock for this protocol syncSession
        final Object messageLock = _messageLocks.get(sessionId);
        if (messageLock == null) {
            Logger.w(CLASS_NAME + " SendMessage error, sesId:" + sessionId);
            handleProtocolError("Error sending protocol message to SYNC.",
                    new SyncException("Attempt to send protocol message prior to startSession ACK.",
                            SyncExceptionCause.SYNC_UNAVAILALBE)
            );
            return;
        }

        final byte protocolVersionToSend = getProtocolVersion();

        ProtocolMessageConverter protocolMessageConverter = new ProtocolMessageConverter(
                protocolMessage, protocolVersionToSend).generate();
        final byte[] data = protocolMessageConverter.getData();
        final ServiceType serviceType = protocolMessageConverter.getServiceType();

        sendProtocolMessageProcessor.process(serviceType, protocolVersionToSend, data,
                MAX_DATA_SIZE, sessionId, getNextMessageId());
    }

    @Override
    public void HandleReceivedBytes(byte[] receivedBytes, int receivedBytesLength) {
        int receivedBytesReadPos = 0;

        Logger.d(CLASS_NAME + " -> Bytes:" + BitConverter.bytesToHex(receivedBytes, 0, receivedBytesLength) +
                ", protocol ver:" + getProtocolVersion());

        Logger.d(CLASS_NAME + " -> current protocol ver:" + getProtocolVersion() +
                " bytes length:" + receivedBytes.length);

        // TODO : Implement proper check point of the Protocol Version
        //Check for a version difference
        if (getProtocolVersion() == ProtocolConstants.PROTOCOL_VERSION_ONE ||
                getProtocolVersion() == ProtocolConstants.PROTOCOL_VERSION_TWO) {
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
                    // TODO - some terrible things is going on. mCurrentHeader.getDataSize()
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
                ASSEMBLER_FOR_SESSION_ID.get(new Byte(header.getSessionId()));
        if (hashSessionID == null) {
            hashSessionID = new Hashtable<Integer, MessageFrameAssembler>();
            ASSEMBLER_FOR_SESSION_ID.put(new Byte(header.getSessionId()), hashSessionID);
        }

        MessageFrameAssembler messageFrameAssembler =
                ASSEMBLER_FOR_MESSAGE_ID.get(new Integer(header.getMessageID()));
        if (messageFrameAssembler == null) {
            messageFrameAssembler = new MessageFrameAssembler(new MessageFrameAssemblerListener() {

                @Override
                public void onHeartbeatACK(byte sessionId) {
                    handleProtocolHeartbeatACK(sessionId);
                }

                @Override
                public void onHeartbeat(byte sessionId) {
                    handleProtocolHeartbeat(sessionId);
                }

                @Override
                public void onStartService(byte sessionId, ServiceType serviceType) {
                    sendProtocolMessageProcessor.processStartSessionAck(serviceType,
                            getProtocolVersion(), sessionId);
                }

                @Override
                public void onStartServiceACK(byte sessionId, int messageId,
                                              ServiceType serviceType, byte protocolVersion) {
                    // Use this sessionID to create a message lock
                    Object messageLock = _messageLocks.get(sessionId);
                    if (messageLock == null) {
                        messageLock = new Object();
                        _messageLocks.put(sessionId, messageLock);
                    }
                    if (protocolVersion >= ProtocolConstants.PROTOCOL_VERSION_TWO) {
                        sessionsHashIds.put(sessionId, messageId);
                    }

                    if (serviceType.equals(ServiceType.RPC)) {
                        handleProtocolSessionStarted(serviceType, sessionId, protocolVersion);
                    } else {
                        handleProtocolServiceStarted(serviceType, sessionId, protocolVersion);
                    }
                }

                @Override
                public void onStartServiceNACK(byte sessionId, ServiceType serviceType) {
                    handleStartServiceNackFrame(sessionId, serviceType);
                }

                @Override
                public void onEndService(byte sessionId, int messageId, ServiceType serviceType) {
                    handleEndServiceFrame(sessionId, messageId, serviceType);
                }

                @Override
                public void onEndServiceNACK(byte sessionId) {

                }

                @Override
                public void onMobileNaviACK(byte sessionId, int messageId) {
                    handleMobileNavAckReceived(sessionId, messageId);
                }

                @Override
                public void onEndServiceACK(byte sessionId, int messageId,  ServiceType serviceType) {
                    handleEndServiceAckFrame(sessionId, messageId, serviceType);
                }

                @Override
                public void onHandleProtocolMessageReceived(int messageId,
                                                            ProtocolMessage protocolMessage) {
                    ASSEMBLER_FOR_MESSAGE_ID.remove(messageId);
                    handleProtocolMessageReceived(protocolMessage);
                }

                @Override
                public void onHandleAppUnregistered() {
                    handleAppUnregistered();
                }
            });
            ASSEMBLER_FOR_MESSAGE_ID.put(new Integer(header.getMessageID()), messageFrameAssembler);
        }

        return messageFrameAssembler;
    }

    private int getNextMessageId() {
        return ++mMessageID;
    }

    private void handleEndServiceFrame(byte sessionId, int messageId, ServiceType serviceType) {
        if (getProtocolVersion() >= ProtocolConstants.PROTOCOL_VERSION_TWO) {
            if (getHashIdBySessionId(sessionId) == messageId) {
                handleProtocolServiceEnded(serviceType, sessionId);
            }
        } else {
            handleProtocolServiceEnded(serviceType, sessionId);
        }
    }

    private void handleEndServiceAckFrame(byte sessionId, int messageId,  ServiceType serviceType) {
        if (getProtocolVersion() >= ProtocolConstants.PROTOCOL_VERSION_TWO) {
            if (getHashIdBySessionId(sessionId) == messageId) {
                handleProtocolServiceEndedAck(serviceType, sessionId);
            }
        } else {
            handleProtocolServiceEndedAck(serviceType, sessionId);
        }
    }

    /**
     * Return Hash Id of the Session. This method declared as public for the Test purposes
     * @param sessionId Id of the Session
     * @return Hash Id
     */
    public int getHashIdBySessionId(byte sessionId) {
        Integer result = sessionsHashIds.get(sessionId);
        if (result == null) {
            result = 0;
        }
        return result;
    }
}