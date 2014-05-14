package com.ford.syncV4.syncConnection;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.protocol.AbstractProtocol;
import com.ford.syncV4.protocol.IProtocolListener;
import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolFrameHeaderFactory;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.FunctionID;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.protocol.heartbeat.IHeartbeatMonitor;
import com.ford.syncV4.protocol.heartbeat.IHeartbeatMonitorListener;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.AppInterfaceUnregisteredReason;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.streaming.AbstractPacketizer;
import com.ford.syncV4.streaming.H264Packetizer;
import com.ford.syncV4.streaming.IStreamListener;
import com.ford.syncV4.test.TestConfig;
import com.ford.syncV4.transport.BTTransport;
import com.ford.syncV4.transport.BaseTransportConfig;
import com.ford.syncV4.transport.ITransportListener;
import com.ford.syncV4.transport.SyncTransport;
import com.ford.syncV4.transport.TCPTransport;
import com.ford.syncV4.transport.TCPTransportConfig;
import com.ford.syncV4.transport.TransportType;
import com.ford.syncV4.transport.nsd.NSDHelper;
import com.ford.syncV4.transport.usb.USBTransport;
import com.ford.syncV4.transport.usb.USBTransportConfig;
import com.ford.syncV4.util.BitConverter;
import com.ford.syncV4.util.logger.Logger;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.util.Hashtable;

/**
 * This class is responsible for the transport connection (Bluetooth, USB, WiFi), provide Services
 * and Session management methods.
 * <p/>
 * When use this class, it is <b>necessary</b> to call 'init( ... )' method to initialize transport
 * connection
 */
public class SyncConnection implements IProtocolListener, ITransportListener, IStreamListener,
        IHeartbeatMonitorListener {
    private static final String CLASS_NAME = SyncConnection.class.getSimpleName();
    SyncTransport _transport = null;
    AbstractProtocol _protocol = null;
    ISyncConnectionListener mConnectionListener = null;
    AbstractPacketizer mVideoPacketizer = null;
    AbstractPacketizer mAudioPacketizer = null;
    // Thread safety locks
    private static final Object TRANSPORT_REFERENCE_LOCK = new Object();
    private static final Object PROTOCOL_REFERENCE_LOCK = new Object();
    private IHeartbeatMonitor mHeartbeatMonitor;
    private boolean mIsHeartbeatTimedout = false;
    private NSDHelper mNSDHelper;

    // Id of the current active session
    private byte mSessionId = Session.DEFAULT_SESSION_ID;
    static final Object END_PROTOCOL_SERVICE_AUDIO_LOCK = new Object();
    static final Object END_PROTOCOL_SERVICE_VIDEO_LOCK = new Object();
    static final Object END_PROTOCOL_SERVICE_RPC_LOCK = new Object();

    private boolean mIsInit = false;

    /**
     * Test Cases fields
     */
    /**
     * This Config object stores all the necessary data for SDK testing
     */
    private TestConfig mTestConfig;

    /**
     * Constructor.
     *
     * @param listener Sync connection listener.
     */
    public SyncConnection(ISyncConnectionListener listener) {
        mConnectionListener = listener;
        mIsInit = false;
    }

    /**
     * Set {@link com.ford.syncV4.test.TestConfig} object
     *
     * @param mTestConfig {@link com.ford.syncV4.test.TestConfig}
     */
    public void setTestConfig(TestConfig mTestConfig) {
        this.mTestConfig = mTestConfig;
    }

    /**
     * Initialize transport with provided configuration
     *
     * @param transportConfig configuration of the transport to be used, refer to
     *                        {@link com.ford.syncV4.transport.BaseTransportConfig}
     */
    public void init(BaseTransportConfig transportConfig) {
        init(transportConfig, null);
    }

    /**
     * Initialize transport with provided configuration and transport instance
     *
     * @param transportConfig configuration of the transport to be used, refer to
     *                        {@link com.ford.syncV4.transport.BaseTransportConfig}
     * @param transport       an instance of transport (Bluetooth, USB, WiFi)
     */
    public void init(BaseTransportConfig transportConfig, SyncTransport transport) {
        // Initialize the transport
        synchronized (TRANSPORT_REFERENCE_LOCK) {
            // Ensure transport is null
            if (_transport != null) {
                if (_transport.getIsConnected()) {
                    _transport.disconnect();
                }
                _transport = null;
            }

            if (transport != null) {
                _transport = transport;
            } else {
                switch (transportConfig.getTransportType()) {
                    case BLUETOOTH:
                        _transport = new BTTransport(this);
                        break;

                    case TCP:
                        TCPTransportConfig tcpTransportConfig = (TCPTransportConfig) transportConfig;
                        _transport = new TCPTransport(tcpTransportConfig, this);
                        if (tcpTransportConfig.getIsNSD()) {
                            mNSDHelper = new NSDHelper(tcpTransportConfig.getApplicationContext());
                            mNSDHelper.initializeNsd();
                        }
                        break;

                    case USB:
                        _transport = new USBTransport((USBTransportConfig) transportConfig, this);
                        break;
                }
            }
        }

        // Initialize the protocol
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            // Ensure protocol is null
            if (_protocol != null) {
                _protocol = null;
            }
            _protocol = new WiProProtocol(this);

            // Apply a value which has been set for the Test Cases
            if (mTestConfig != null) {
                _protocol.set_TEST_ProtocolMinVersion(mTestConfig.getProtocolMinVersion());
                _protocol.set_TEST_ProtocolMaxVersion(mTestConfig.getProtocolMaxVersion());
            }
        }

        mIsInit = true;
    }

    public AbstractProtocol getWiProProtocol() {
        return _protocol;
    }

    public IHeartbeatMonitor getHeartbeatMonitor() {
        return mHeartbeatMonitor;
    }

    public void setHeartbeatMonitor(IHeartbeatMonitor heartbeatMonitor) {
        mHeartbeatMonitor = heartbeatMonitor;
        mHeartbeatMonitor.setListener(this);
    }

    private void stopTransportReading() {
        if (_transport != null) {
            _transport.stopReading();
        }
    }

    public void closeConnection(byte rpcSessionID, boolean keepConnection) {
        closeConnection(rpcSessionID, keepConnection, true);
    }

    public void closeConnection(byte rpcSessionID, boolean keepConnection,
                                boolean sendFinishMessages) {
        if (rpcSessionID != 0) {
            synchronized (PROTOCOL_REFERENCE_LOCK) {
                if (!getIsConnected()) {
                    return;
                }
                if (_protocol != null) {
                    // If transport is still connected, sent EndProtocolSessionMessage
                    if (sendFinishMessages) {
                        _protocol.EndProtocolService(ServiceType.RPC, rpcSessionID);
                        stopHeartbeatMonitor();
                    }
                }
            }

            waitForRpcEndServiceACK();
        }

        /*synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (!keepConnection) {
                _protocol = null;
            }
        }*/

        Logger.d("Close connection:" + keepConnection);
        synchronized (TRANSPORT_REFERENCE_LOCK) {

            stopH264();
            stopAudioDataTransfer();

            if (!keepConnection) {

                if (mNSDHelper != null) {
                    mNSDHelper.stopDiscovery();
                    mNSDHelper.tearDown();
                }

                if (_transport != null) {
                    _transport.disconnect();

                    // TODO : Add here 'shutdownAllExecutors' of the 'SendProtocolMessageProcessor'
                    // TODO : method call
                }
                _transport = null;
            }
        }
    }

    private void stopHeartbeatMonitor() {
        if (mHeartbeatMonitor != null) {
            Logger.d(CLASS_NAME + " Stop HeartBeat");
            mHeartbeatMonitor.stop();
        }
    }

    private void waitForRpcEndServiceACK() {
        synchronized (END_PROTOCOL_SERVICE_RPC_LOCK) {
            try {
                END_PROTOCOL_SERVICE_RPC_LOCK.wait(1000);
            } catch (InterruptedException e) {
                // Do nothing
            }
        }
    }

    public void closeMobileNaviService(byte mobileNavSessionId) {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (!getIsConnected()) {
                return;
            }
            if (_protocol == null) {
                return;
            }
            _protocol.EndProtocolService(ServiceType.Mobile_Nav, mobileNavSessionId);
        }
        waitForVideoEndServiceACK();
    }

    private void waitForVideoEndServiceACK() {
        synchronized (END_PROTOCOL_SERVICE_VIDEO_LOCK) {
            try {
                END_PROTOCOL_SERVICE_VIDEO_LOCK.wait(1000);
            } catch (InterruptedException e) {
                // Do nothing
            }
        }
    }

    public void closeAudioService(byte sessionID) {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (!getIsConnected()) {
                return;
            }
            if (_protocol == null) {
                return;
            }
            _protocol.EndProtocolService(ServiceType.Audio_Service, sessionID);
        }
        waitForAudioEndServiceACK();
    }

    private void waitForAudioEndServiceACK() {
        synchronized (END_PROTOCOL_SERVICE_AUDIO_LOCK) {
            try {
                END_PROTOCOL_SERVICE_AUDIO_LOCK.wait(1000);
            } catch (InterruptedException e) {
                // Do nothing
            }
        }
    }

    public OutputStream startH264(byte rpcSessionID) {
        try {
            OutputStream os = new PipedOutputStream();
            InputStream is = new PipedInputStream((PipedOutputStream) os);
            mVideoPacketizer = new H264Packetizer(this, is, rpcSessionID, ServiceType.Mobile_Nav);
            mVideoPacketizer.start();
            return os;
        } catch (Exception e) {
            Logger.e(CLASS_NAME + " Unable to start H.264 streaming:" + e.toString());
        }
        return null;
    }

    public void stopH264() {
        if (mVideoPacketizer != null) {
            mVideoPacketizer.stop();
        }
    }

    public OutputStream startAudioDataTransfer(byte rpcSessionID) {
        try {
            OutputStream os = new PipedOutputStream();
            InputStream is = new PipedInputStream((PipedOutputStream) os);
            mAudioPacketizer = new H264Packetizer(this, is, rpcSessionID, ServiceType.Audio_Service);
            mAudioPacketizer.start();
            return os;
        } catch (IOException e) {
            Logger.e(CLASS_NAME + " Unable to start audio streaming:" + e.toString());
        }
        return null;
    }

    public void stopAudioDataTransfer() {
        if (mAudioPacketizer != null) {
            mAudioPacketizer.stop();
        }
    }

    /**
     * Start a connection of the provided or specified transport
     *
     * @throws SyncException
     */
    public void startTransport() throws SyncException {
        if (!mIsInit) {
            throw new SyncException("You must call 'init( ... )' method before start transport",
                    SyncExceptionCause.SYNC_CONNECTION_INIT_EXCEPTION);
        }
        _transport.openConnection();
    }

    public Boolean getIsConnected() {
        if (_transport == null) {
            return false;
        }
        return _transport.getIsConnected();
    }

    public void sendMessage(ProtocolMessage msg) {
        if (!getIsConnected()) {
            return;
        }
        if (_protocol == null) {
            return;
        }
        if (msg == null) {
            return;
        }
        _protocol.SendMessage(msg);
    }

    public void startMobileNavService(Session session) {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (!getIsConnected()) {
                return;
            }
            if (_protocol == null) {
                return;
            }
            _protocol.StartProtocolService(ServiceType.Mobile_Nav, session);
        }
    }

    public void startAudioService(Session session) {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (!getIsConnected()) {
                return;
            }
            if (_protocol == null) {
                return;
            }
            _protocol.StartProtocolService(ServiceType.Audio_Service, session);
        }
    }

    @Override
    public void onTransportBytesReceived(byte[] receivedBytes, int receivedBytesLength) {
        // Send bytes to protocol to be interpreted
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (!getIsConnected()) {
                return;
            }
            if (_protocol == null) {
                return;
            }
            try {
                _protocol.HandleReceivedBytes(receivedBytes, receivedBytesLength);
            } catch (OutOfMemoryError e) {
                final String info = " Out of memory while handling incoming message";
                if (mConnectionListener != null) {
                    mConnectionListener.onProtocolError(info, e);
                } else {
                    Logger.e(CLASS_NAME + info, e);
                }
            }
        }
    }

    @Override
    public void onTransportConnected() {
        if (mConnectionListener != null) {
            mConnectionListener.onTransportConnected();
        }
    }

    public void initialiseSession(byte sessionId) {
        startProtocolSession(sessionId);
    }

    public void startHeartbeatTimer() {
        if (mHeartbeatMonitor != null) {
            mHeartbeatMonitor.start();
        }
    }

    private void startProtocolSession(byte sessionId) {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (!getIsConnected()) {
                return;
            }
            if (_protocol == null) {
                return;
            }
            Logger.d(CLASS_NAME + " StartProtocolSession, id:" + sessionId);
            _protocol.StartProtocolSession(sessionId);
        }
    }

    /**
     * Return the version of protocol
     *
     * @return byte value of the protocol version
     */
    public byte getProtocolVersion() {
        return _protocol.getProtocolVersion();
    }

    @Override
    public void onTransportDisconnected(String info) {
        if (!mIsHeartbeatTimedout) {
            // Pass directly to connection listener
            mConnectionListener.onTransportDisconnected(info);
        }
        mIsHeartbeatTimedout = false;
    }

    @Override
    public void onTransportError(String info, Exception e) {
        if (!mIsHeartbeatTimedout) {
            // Pass directly to connection listener
            mConnectionListener.onTransportError(info, e);
        }
        mIsHeartbeatTimedout = false;
    }

    @Override
    public void onServerSocketInit(int serverSocketPort) {
        Logger.d("SyncConnection", "ServerSocket init: " + serverSocketPort);
        mNSDHelper.registerService(serverSocketPort);
        mNSDHelper.discoverServices();
    }

    @Override
    public void onProtocolMessageBytesToSend(byte[] msgBytes, int offset, int length) {
        // Protocol has packaged bytes to send, pass to transport for transmission
        synchronized (TRANSPORT_REFERENCE_LOCK) {
            if (_transport != null) {
                Logger.d(CLASS_NAME + " <- Bytes:" + BitConverter.bytesToHex(msgBytes));
                _transport.sendBytes(msgBytes, offset, length);
            }
        }
    }

    @Override
    public void onProtocolMessageReceived(ProtocolMessage msg) {
        mConnectionListener.onProtocolMessageReceived(msg);

        // Unblock USB reader thread by this method
        FunctionID functionID = new FunctionID();
        String functionName = functionID.getFunctionName(msg.getFunctionID());
        if (functionName == null) {
            return;
        }
        if (functionName.equals(Names.OnAppInterfaceUnregistered)) {
            IJsonRPCMarshaller marshaller = new JsonRPCMarshaller();
            Hashtable<String, Object> hashtable = marshaller.unmarshall(msg.getData());
            if (hashtable.containsKey(Names.reason)) {
                String reason = hashtable.get(Names.reason).toString();
                if (reason == null) {
                    return;
                }
                if (reason.equals(AppInterfaceUnregisteredReason.IGNITION_OFF.toString()) ||
                        reason.equals(AppInterfaceUnregisteredReason.MASTER_RESET.toString()) ||
                        reason.equals(AppInterfaceUnregisteredReason.FACTORY_DEFAULTS.toString())) {
                    processTransportStopReading();
                }
            }
        }
    }

    @Override
    public void onProtocolSessionStarted(byte sessionId, byte version, String correlationID) {
        mConnectionListener.onProtocolSessionStarted(sessionId, version, correlationID);
    }

    @Override
    public void onProtocolServiceEnded(ServiceType serviceType, byte sessionID,
                                       String correlationID) {
        mConnectionListener.onProtocolServiceEnded(serviceType, sessionID, correlationID);
        processEndService(serviceType);
    }

    private void processEndService(ServiceType serviceType) {
        if (_transport != null && serviceType.equals(ServiceType.RPC)) {
            synchronized (END_PROTOCOL_SERVICE_RPC_LOCK) {
                END_PROTOCOL_SERVICE_RPC_LOCK.notifyAll();
            }

            processTransportStopReading();

        } else if (_transport != null && serviceType.equals(ServiceType.Mobile_Nav)) {
            synchronized (END_PROTOCOL_SERVICE_VIDEO_LOCK) {
                END_PROTOCOL_SERVICE_VIDEO_LOCK.notifyAll();
            }
        } else if (_transport != null && serviceType.equals(ServiceType.Audio_Service)) {
            synchronized (END_PROTOCOL_SERVICE_AUDIO_LOCK) {
                END_PROTOCOL_SERVICE_AUDIO_LOCK.notifyAll();
            }
        }
    }

    @Override
    public void onProtocolHeartbeatACK() {
        if (mHeartbeatMonitor != null) {
            mHeartbeatMonitor.heartbeatACKReceived();
        }
    }

    @Override
    public void onProtocolHeartbeat() {
        if (mHeartbeatMonitor != null) {
            mHeartbeatMonitor.heartbeatReceived();
        }
    }

    @Override
    public void onResetHeartbeatAck() {
        if (mHeartbeatMonitor != null) {
            mHeartbeatMonitor.notifyTransportOutputActivity();
        }
    }

    @Override
    public void onResetHeartbeat() {
        if (mHeartbeatMonitor != null) {
            mHeartbeatMonitor.notifyTransportInputActivity();
        }
    }

    @Override
    public void onProtocolError(String info, Exception e) {
        mConnectionListener.onProtocolError(info, e);
    }

    @Override
    public void onProtocolAppUnregistered() {
        Logger.d(CLASS_NAME + " onProtocolAppUnregistered");
    }

    @Override
    public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, byte version, String correlationID) {
        mConnectionListener.onProtocolServiceStarted(serviceType, sessionID, version, correlationID);
    }

    @Override
    public void onMobileNavAckReceived(int frameReceivedNumber) {
        mConnectionListener.onMobileNavAckReceived(frameReceivedNumber);
    }

    @Override
    public void onStartServiceNackReceived(ServiceType serviceType) {
        mConnectionListener.onStartServiceNackReceived(serviceType);
    }

    /**
     * Gets type of transport currently used by this connection.
     *
     * @return One of TransportType enumeration values.
     * @see TransportType
     */
    public TransportType getCurrentTransportType() {
        return _transport.getTransportType();
    }

    @Override
    public void sendH264(ProtocolMessage pm) {
        sendMessage(pm);
    }

    @Override
    public void sendHeartbeat(IHeartbeatMonitor monitor) {
        Logger.d(CLASS_NAME + " Asked to send heartbeat");
        _protocol.SendHeartBeatMessage(getSessionId());
    }

    @Override
    public void heartbeatTimedOut(IHeartbeatMonitor monitor) {
        Logger.d(CLASS_NAME + " Heartbeat timeout; closing connection");
        mIsHeartbeatTimedout = true;
        closeConnection((byte) 0, false, true);
        mConnectionListener.onHeartbeatTimedOut();
    }

    @Override
    public void sendHeartbeatACK(IHeartbeatMonitor heartbeatMonitor) {
        Logger.d(CLASS_NAME + " Asked to send heartbeat ack");
        _protocol.SendHeartBeatAckMessage(getSessionId());
    }

    public byte getSessionId() {
        return mSessionId;
    }

    /**
     * Set ID of the current active session
     *
     * @param sessionId
     */
    public void setSessionId(byte sessionId) {
        mSessionId = sessionId;
        Logger.d(CLASS_NAME + " SetSessionId:" + mSessionId);
    }

    private void processTransportStopReading() {
        if (_transport == null) {
            Logger.w(CLASS_NAME + " Process Transport Stop Reading - transport is NULL");
            return;
        }

        /**
         * TODO : For the TestCases
         */
        boolean doStopReading = true;
        if (mTestConfig != null && mTestConfig.isDoKeepUSBTransportConnected()) {
            doStopReading = false;
        }

        if (doStopReading) {
            _transport.stopReading();
        }

        if (mTestConfig != null) {
            mTestConfig.setDoKeepUSBTransportConnected(false);
        }

        //_transport.stopReading();
    }
}