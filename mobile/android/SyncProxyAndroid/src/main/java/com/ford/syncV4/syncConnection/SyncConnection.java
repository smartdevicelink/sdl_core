package com.ford.syncV4.syncConnection;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.protocol.AbstractProtocol;
import com.ford.syncV4.protocol.IProtocolListener;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.FunctionID;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.protocol.heartbeat.HeartbeatMonitorsManager;
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
public class SyncConnection implements IProtocolListener, ITransportListener {
    private static final String CLASS_NAME = SyncConnection.class.getSimpleName();
    SyncTransport mTransport = null;
    AbstractProtocol mProtocol = null;
    ISyncConnectionListener mConnectionListener = null;
    AbstractPacketizer mVideoPacketizer = null;
    AbstractPacketizer mAudioPacketizer = null;
    // Thread safety locks
    private static final Object TRANSPORT_REFERENCE_LOCK = new Object();
    private static final Object PROTOCOL_REFERENCE_LOCK = new Object();

    private volatile HeartbeatMonitorsManager heartbeatMonitorsManager = new HeartbeatMonitorsManager();
    private NSDHelper mNSDHelper;

    static final Object END_PROTOCOL_SERVICE_AUDIO_LOCK = new Object();
    static final Object END_PROTOCOL_SERVICE_VIDEO_LOCK = new Object();
    static final Object END_PROTOCOL_SERVICE_RPC_LOCK = new Object();
    static final Object START_PROTOCOL_SESSION_LOCK = new Object();
    static final Object START_SERVICE_LOCK = new Object();

    /**
     * Waiting time for the End Service ACK, in milliseconds
     */
    private static final int WAIT_END_TIMEOUT = 1000;

    private boolean mIsInit = false;
    private Session mSyncSession;
    private BaseTransportConfig mTransportConfig;

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
     * @param transportConfig configuration of the transport to be used, refer to
     *                        {@link com.ford.syncV4.transport.BaseTransportConfig}
     * @param listener Sync connection listener
     */
    public SyncConnection(Session session, BaseTransportConfig transportConfig,
                          ISyncConnectionListener listener) {
        mConnectionListener = listener;
        mIsInit = false;
        mTransportConfig = transportConfig;
        mSyncSession = session;
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
     * Initialize transport with provided configuration and transport instance
     */
    public void init() {
        // Initialize the transport
        Logger.d(CLASS_NAME + " Init");
        synchronized (TRANSPORT_REFERENCE_LOCK) {
            switch (mTransportConfig.getTransportType()) {
                case BLUETOOTH:
                    mTransport = new BTTransport(this);
                    break;
                case TCP:
                    TCPTransportConfig tcpTransportConfig = (TCPTransportConfig) mTransportConfig;
                    mTransport = new TCPTransport(tcpTransportConfig, this);
                    if (tcpTransportConfig.getIsNSD()) {
                        mNSDHelper = new NSDHelper(tcpTransportConfig.getApplicationContext());
                        mNSDHelper.initializeNsd();
                    }
                    break;
                case USB:
                    mTransport = new USBTransport((USBTransportConfig) mTransportConfig, this);
                    break;
            }
        }

        // Initialize the protocol
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            // Ensure protocol is null
            if (mProtocol != null) {
                Logger.w(CLASS_NAME + " protocol should be null here");
            }
            mProtocol = new WiProProtocol(this);

            // Apply a value which has been set for the Test Cases
            if (mTestConfig != null) {
                mProtocol.set_TEST_ProtocolMinVersion(mTestConfig.getProtocolMinVersion());
                mProtocol.set_TEST_ProtocolMaxVersion(mTestConfig.getProtocolMaxVersion());
            }
        }

        mIsInit = true;
    }

    public AbstractProtocol getWiProProtocol() {
        return mProtocol;
    }

    public void closeConnection() {
        Logger.d("Close connection");
        synchronized (TRANSPORT_REFERENCE_LOCK) {

            heartbeatMonitorsManager.dispose();

            stopH264();
            stopAudioDataTransfer();

            if (mNSDHelper != null) {
                mNSDHelper.stopDiscovery();
                mNSDHelper.tearDown();
            }

            if (mTransport != null) {
                mTransport.disconnect();
                //mTransport.removeListener();
            }
            //mTransport = null;

            /*if (mProtocol != null) {
                mProtocol.removeListener();
            }*/
            //mProtocol = null;
        }
    }

    public void closeSession(byte sessionId) {
        if (sessionId == Session.DEFAULT_SESSION_ID || sessionId == Session.UNDEFINED_SESSION_ID) {
            return;
        }
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (mProtocol == null) {
                return;
            }
            // If transport is still connected, sent EndProtocolSessionMessage
            if (!getIsConnected()) {
                return;
            }
            mProtocol.EndProtocolService(ServiceType.RPC, sessionId);
        }
        waitForRpcEndServiceACK();
    }

    public synchronized void stopAllHeartbeatMonitors() {
        heartbeatMonitorsManager.stopAllMonitors();
    }

    public synchronized void stopHeartbeatMonitor(byte sessionId) {
        heartbeatMonitorsManager.stopMonitor(sessionId);
    }

    private void waitForRpcEndServiceACK() {
        synchronized (END_PROTOCOL_SERVICE_RPC_LOCK) {
            try {
                END_PROTOCOL_SERVICE_RPC_LOCK.wait(WAIT_END_TIMEOUT);
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
            if (mProtocol == null) {
                return;
            }
            mProtocol.EndProtocolService(ServiceType.Mobile_Nav, mobileNavSessionId);
        }
        waitForVideoEndServiceACK();
    }

    private void waitForVideoEndServiceACK() {
        synchronized (END_PROTOCOL_SERVICE_VIDEO_LOCK) {
            try {
                END_PROTOCOL_SERVICE_VIDEO_LOCK.wait(WAIT_END_TIMEOUT);
            } catch (InterruptedException e) {
                // Do nothing
            }
        }
    }

    public void closeAudioService(byte sessionId) {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (!getIsConnected()) {
                return;
            }
            if (mProtocol == null) {
                return;
            }
            mProtocol.EndProtocolService(ServiceType.Audio_Service, sessionId);
        }
        waitForAudioEndServiceACK();
    }

    private void waitForAudioEndServiceACK() {
        synchronized (END_PROTOCOL_SERVICE_AUDIO_LOCK) {
            try {
                END_PROTOCOL_SERVICE_AUDIO_LOCK.wait(WAIT_END_TIMEOUT);
            } catch (InterruptedException e) {
                // Do nothing
            }
        }
    }

    public OutputStream startH264(byte rpcSessionID, boolean encrypt) {
        try {
            OutputStream os = new PipedOutputStream();
            InputStream is = new PipedInputStream((PipedOutputStream) os);
            mVideoPacketizer = new H264Packetizer(new IStreamListener() {
                @Override
                public void sendH264(ProtocolMessage protocolMessage) {
                    if (protocolMessage != null) {
                        sendMessage(protocolMessage);
                    }
                }
            }, is, rpcSessionID, ServiceType.Mobile_Nav, encrypt);
            mVideoPacketizer.start();
            return os;
        } catch (Exception e) {
            Logger.e(CLASS_NAME + " Unable to start H.264 streaming:" + e.toString());
        }
        return null;
    }

    public void stopH264() {
        try {
            if (mProtocol != null) {
                ((WiProProtocol)mProtocol).shutDownMobileNaviStreamExecutor();
            }
        } catch (InterruptedException e) {
            Logger.e(CLASS_NAME + " Stop Mobile Navi exception:" + e.getMessage());
        }
        if (mVideoPacketizer != null) {
            mVideoPacketizer.removeListener();
            mVideoPacketizer.stop();
        }
    }

    public OutputStream startAudioDataTransfer(byte rpcSessionID, boolean encrypt) {
        try {
            OutputStream os = new PipedOutputStream();
            InputStream is = new PipedInputStream((PipedOutputStream) os);
            mAudioPacketizer = new H264Packetizer(new IStreamListener() {
                @Override
                public void sendH264(ProtocolMessage protocolMessage) {
                    if (protocolMessage != null) {
                        sendMessage(protocolMessage);
                    }
                }
            }, is, rpcSessionID, ServiceType.Audio_Service, encrypt);
            mAudioPacketizer.start();
            return os;
        } catch (IOException e) {
            Logger.e(CLASS_NAME + " Unable to start audio streaming:" + e.toString());
        }
        return null;
    }

    public void stopAudioDataTransfer() {
        try {
            if (mProtocol != null) {
                ((WiProProtocol)mProtocol).shutDownAudioStreamExecutor();
            }
        } catch (InterruptedException e) {
            Logger.e(CLASS_NAME + " Stop Audio exception:" + e.getMessage());
        }
        if (mAudioPacketizer != null) {
            mAudioPacketizer.removeListener();
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
        mTransport.openConnection();
    }

    public Boolean getIsConnected() {
        //Logger.d(CLASS_NAME + " get is connected, tr:" + mTransport);
        if (mTransport == null) {
            return false;
        }
        //Logger.d(CLASS_NAME + " get is connected, tr con:" + mTransport.getIsConnected());
        return mTransport.getIsConnected();
    }

    public void sendMessage(ProtocolMessage msg) {
        if (!getIsConnected()) {
            return;
        }
        if (mProtocol == null) {
            return;
        }
        if (msg == null) {
            return;
        }
        mProtocol.SendMessage(msg);
    }

    public void startMobileNavService(byte sessionId,  boolean isCyphered) {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (!getIsConnected()) {
                return;
            }
            if (mProtocol == null) {
                return;
            }
            mProtocol.StartProtocolService(ServiceType.Mobile_Nav, sessionId, isCyphered);
            synchronized (START_SERVICE_LOCK){
                try {
                    START_SERVICE_LOCK.wait(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public void startAudioService(byte sessionId,boolean isCyphered) {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (!getIsConnected()) {
                return;
            }
            if (mProtocol == null) {
                return;
            }
            mProtocol.StartProtocolService(ServiceType.Audio_Service, sessionId, isCyphered);
            synchronized (START_SERVICE_LOCK){
                try {
                    START_SERVICE_LOCK.wait(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public void startRpcService(byte sessionId, boolean isCyphered) {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (!getIsConnected()) {
                return;
            }
            if (mProtocol == null) {
                return;
            }
            mProtocol.StartProtocolService(ServiceType.RPC, sessionId, isCyphered);
            synchronized (START_SERVICE_LOCK){
                try {
                    START_SERVICE_LOCK.wait(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    @Override
    public void onTransportBytesReceived(byte[] receivedBytes, int receivedBytesLength) {
        // Send bytes to protocol to be interpreted
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (mProtocol == null) {
                return;
            }
            try {
                //Logger.d(CLASS_NAME + " -> Bytes:" + receivedBytes.length);
                mProtocol.HandleReceivedBytes(receivedBytes, receivedBytesLength);
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

    public void addHeartbeatMonitor(IHeartbeatMonitor monitor) {
        heartbeatMonitorsManager.addMonitor(monitor, SyncConnection.class.getSimpleName());
    }

    public void addHeartbeatMonitor(byte sessionId, int heartBeatInterval, boolean heartBeatAck) {
        heartbeatMonitorsManager.addMonitor(sessionId, heartBeatInterval, heartBeatAck);
    }

    public void startHeartbeatMonitor(byte sessionId) {
        heartbeatMonitorsManager.startMonitor(sessionId, new IHeartbeatMonitorListener() {

            @Override
            public void sendHeartbeat(IHeartbeatMonitor monitor) {
                Logger.d(CLASS_NAME + " Asked to send heartbeat, sesId:" + monitor.getSessionId());
                mProtocol.SendHeartBeatMessage(monitor.getSessionId());
            }

            @Override
            public void heartbeatTimedOut(IHeartbeatMonitor monitor) {
                Logger.d(CLASS_NAME + " Heartbeat timeout, sesId:" + monitor.getSessionId());
                mConnectionListener.onHeartbeatTimedOut(monitor.getSessionId());
            }

            @Override
            public void sendHeartbeatACK(IHeartbeatMonitor monitor) {
                Logger.d(CLASS_NAME + " Asked to send heartbeat ack, sesId:" + monitor.getSessionId());
                mProtocol.SendHeartBeatAckMessage(monitor.getSessionId());
            }
        });
    }

    private void startProtocolSession(byte sessionId) {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (!getIsConnected()) {
                return;
            }
            if (mProtocol == null) {
                return;
            }
            Logger.d(CLASS_NAME + " StartProtocolSession, id:" + sessionId);
            mProtocol.StartProtocolSession(sessionId);

            synchronized (START_PROTOCOL_SESSION_LOCK) {
                try {
                    START_PROTOCOL_SESSION_LOCK.wait(1000);
                } catch (InterruptedException e) {

                }
            }
        }
    }

    /**
     * Return the version of protocol
     *
     * @return byte value of the protocol version
     */
    public byte getProtocolVersion() {
        return mProtocol.getProtocolVersion();
    }

    @Override
    public void onTransportDisconnected(String info) {
        // Pass directly to connection listener
        mConnectionListener.onTransportDisconnected(info);
    }

    @Override
    public void onTransportError(String info, Exception e) {
        // Pass directly to connection listener
        mConnectionListener.onTransportError(info, e);
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
            if (mTransport != null) {
                //Logger.d(CLASS_NAME + " <- Bytes:" + BitConverter.bytesToHex(msgBytes));
                mTransport.sendBytes(msgBytes, offset, length);
            }
        }
    }

    @Override
    public void onProtocolMessageReceived(ProtocolMessage msg) {
        mConnectionListener.onProtocolMessageReceived(msg);

        // Unblock USB reader thread by this method
        String functionName = FunctionID.getFunctionName(msg.getFunctionID());
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
    public void onProtocolSessionStarted(byte sessionId, byte version, boolean encrypted) {
        mConnectionListener.onProtocolSessionStarted(sessionId, version, encrypted);
        synchronized (START_PROTOCOL_SESSION_LOCK) {
            START_PROTOCOL_SESSION_LOCK.notify();
        }
    }

    @Override
    public void onProtocolServiceEnded(ServiceType serviceType, byte sessionId) {
        mConnectionListener.onProtocolServiceEnded(serviceType, sessionId);
    }

    @Override
    public void onProtocolServiceEndedAck(ServiceType serviceType, byte sessionId) {
        mConnectionListener.onProtocolServiceEndedAck(serviceType, sessionId);
        processEndService(serviceType);
    }

    private void processEndService(ServiceType serviceType) {
        if (mTransport == null ) {
            Logger.w("ProcessEndService transport null");
            return;
        }
        if (serviceType.equals(ServiceType.RPC)) {
            synchronized (END_PROTOCOL_SERVICE_RPC_LOCK) {
                END_PROTOCOL_SERVICE_RPC_LOCK.notifyAll();
            }

            if (mSyncSession.getSessionIdsNumber() == 1) {
                processTransportStopReading();
            }

        } else if (serviceType.equals(ServiceType.Mobile_Nav)) {
            synchronized (END_PROTOCOL_SERVICE_VIDEO_LOCK) {
                END_PROTOCOL_SERVICE_VIDEO_LOCK.notifyAll();
            }
        } else if (serviceType.equals(ServiceType.Audio_Service)) {
            synchronized (END_PROTOCOL_SERVICE_AUDIO_LOCK) {
                END_PROTOCOL_SERVICE_AUDIO_LOCK.notifyAll();
            }
        }
    }

    @Override
    public void onProtocolHeartbeatACK(byte sessionId) {
        heartbeatMonitorsManager.heartbeatACKReceived(sessionId);
    }

    @Override
    public void onProtocolHeartbeat(byte sessionId) {
        heartbeatMonitorsManager.heartbeatReceived(sessionId);
    }

    @Override
    public void onResetHeartbeatAck(byte sessionId) {
        heartbeatMonitorsManager.notifyTransportOutputActivity(sessionId);
    }

    @Override
    public void onResetHeartbeat(byte sessionId) {
        heartbeatMonitorsManager.notifyTransportInputActivity(sessionId);
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
    public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, boolean encrypted, byte version){
        mConnectionListener.onProtocolServiceStarted(serviceType, sessionID, encrypted,version);
        synchronized (START_SERVICE_LOCK){
            START_SERVICE_LOCK.notify();
        }
    }

    @Override
    public void onMobileNavAckReceived(byte sessionId, int frameReceivedNumber) {
        mConnectionListener.onMobileNavAckReceived(sessionId, frameReceivedNumber);
    }

    @Override
    public void onStartServiceNackReceived(byte sessionId, ServiceType serviceType) {
        mConnectionListener.onStartServiceNackReceived(sessionId, serviceType);
    }

    /**
     * Gets type of transport currently used by this connection.
     *
     * @return One of TransportType enumeration values.
     * @see TransportType
     */
    public TransportType getCurrentTransportType() {
        return mTransport.getTransportType();
    }

    private void processTransportStopReading() {
        if (mTransport == null) {
            Logger.w("ProcessTransportStopReading transport null");
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
            mTransport.stopReading();
        }

        if (mTestConfig != null) {
            mTestConfig.setDoKeepUSBTransportConnected(false);
        }
    }


}