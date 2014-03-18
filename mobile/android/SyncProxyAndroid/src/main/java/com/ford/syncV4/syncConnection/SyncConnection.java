package com.ford.syncV4.syncConnection;

import android.util.Log;

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

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.util.Hashtable;

/**
 * This class is responsible for the transport connection (Bluetooth, USB, WiFi), provide Services
 * and Session management methods.
 *
 * When use this class, it is <b>necessary</b> to call 'init( ... )' method to initialize transport
 * connection
 */
public class SyncConnection implements IProtocolListener, ITransportListener, IStreamListener,
        IHeartbeatMonitorListener {
    private static final String TAG = "SyncConnection";

    public SyncTransport getTransport() {
        return _transport;
    }

    SyncTransport _transport = null;
    AbstractProtocol _protocol = null;
    ISyncConnectionListener _connectionListener = null;
    AbstractPacketizer mVideoPacketizer = null;
    AbstractPacketizer mAudioPacketizer = null;
    // Thread safety locks
    private static final Object TRANSPORT_REFERENCE_LOCK = new Object();
    private static final Object PROTOCOL_REFERENCE_LOCK = new Object();
    IHeartbeatMonitor _heartbeatMonitor;
    private boolean _isHeartbeatTimedout = false;
    private NSDHelper mNSDHelper;

    // Id of the current active session
    private byte mSessionId = Session.DEFAULT_SESSION_ID;
    static final Object END_PROTOCOL_SERVICE_AUDIO_LOCK = new Object();
    static final Object END_PROTOCOL_SERVICE_VIDEO_LOCK = new Object();
    static final Object END_PROTOCOL_SERVICE_RPC_LOCK = new Object();

    private boolean mIsInit = false;

    /**
     * Constructor.
     *
     * @param listener Sync connection listener.
     */
    public SyncConnection(ISyncConnectionListener listener) {
        _connectionListener = listener;
        mIsInit = false;
    }

    /**
     * Initialize transport with provided configuration
     *
     * @param transportConfig configuration of the transport to be used, refer to
     * {@link com.ford.syncV4.transport.BaseTransportConfig}
     */
    public void init(BaseTransportConfig transportConfig) {
        init(transportConfig, null);
    }

    /**
     * Initialize transport with provided configuration and transport instance
     *
     * @param transportConfig configuration of the transport to be used, refer to
     * {@link com.ford.syncV4.transport.BaseTransportConfig}
     * @param transport an instance of transport (Bluetooth, USB, WiFi)
     */
    public void init(BaseTransportConfig transportConfig, SyncTransport transport) {
        init(transportConfig, transport, null);
    }

    /**
     * Initialize transport with provided configuration and transport instance
     *
     * @param transportConfig configuration of the transport to be used, refer to
     * {@link com.ford.syncV4.transport.BaseTransportConfig}
     * @param transport an instance of transport (Bluetooth, USB, WiFi)
     * @param protocol an instance of {@link com.ford.syncV4.protocol.AbstractProtocol}
     *                 implementation
     */
    public void init(BaseTransportConfig transportConfig, SyncTransport transport,
                     AbstractProtocol protocol) {
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
        }

        mIsInit = true;
    }

    public AbstractProtocol getWiProProtocol() {
        return _protocol;
    }

    public IHeartbeatMonitor getHeartbeatMonitor() {
        return _heartbeatMonitor;
    }

    public void setHeartbeatMonitor(IHeartbeatMonitor heartbeatMonitor) {
        this._heartbeatMonitor = heartbeatMonitor;
        _heartbeatMonitor.setListener(this);
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
                if (_protocol != null) {
                    // If transport is still connected, sent EndProtocolSessionMessage
                    if (sendFinishMessages && (_transport != null) && _transport.getIsConnected()) {
                        _protocol.EndProtocolService(ServiceType.RPC, rpcSessionID);
                    }
                }
            }

            waitForRpcEndServiceACK();
        }

        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (!keepConnection) {
                _protocol = null;
            }
        }
        if (!keepConnection) {
            stopHeartbeatMonitor();
        }
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
                }
                _transport = null;
            }
        }
    }

    public void stopHeartbeatMonitor() {
        if (_heartbeatMonitor != null) {
            _heartbeatMonitor.stop();
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
            if (_protocol != null) {
                // If transport is still connected, sent EndProtocolSessionMessage
                if (_transport != null && _transport.getIsConnected()) {
                    _protocol.EndProtocolService(ServiceType.Mobile_Nav, mobileNavSessionId);
                }
            } // end-if
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
            if (_protocol != null) {
                // If transport is still connected, sent EndProtocolSessionMessage
                if (_transport != null && _transport.getIsConnected()) {
                    _protocol.EndProtocolService(ServiceType.Audio_Service, sessionID);
                }
            } // end-if
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
            Log.e(TAG, "Unable to start H.264 streaming:" + e.toString());
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
            Log.e(TAG, "Unable to start audio streaming:" + e.toString());
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
        if (msg != null && _protocol != null) {
            _protocol.SendMessage(msg);
        }
    }

    public void startMobileNavService(Session session) {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (_protocol != null) {
                _protocol.StartProtocolService(ServiceType.Mobile_Nav, session);
            }
        }
    }

    public void startAudioService(Session session) {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (_protocol != null) {
                _protocol.StartProtocolService(ServiceType.Audio_Service, session);
            }
        }
    }

    @Override
    public void onTransportBytesReceived(byte[] receivedBytes, int receivedBytesLength) {
        // Send bytes to protocol to be interpreted
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (_protocol != null) {
                try {
                    _protocol.HandleReceivedBytes(receivedBytes, receivedBytesLength);
                } catch (OutOfMemoryError e) {
                    final String info = "Out of memory while handling incoming message";
                    if (_connectionListener != null) {
                        _connectionListener.onProtocolError(info, e);
                    } else {
                        Log.e(TAG, info, e);
                    }
                }
            }
        }
    }

    @Override
    public void onTransportConnected() {
        initialiseSession();
    }

    private void initialiseSession() {
        if (_heartbeatMonitor != null) {
            _heartbeatMonitor.start();
        }else {

        }

        startProtocolSession();
    }

    public void startSecureService() {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (_protocol != null) {
                Log.d(TAG, "Start Secure Service, session id:" + mSessionId);
                _protocol.startSecureService(mSessionId);
            }
        }
    }

    private void startProtocolSession() {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (_protocol != null) {
                Log.d(TAG, "StartProtocolSession, session id:" + mSessionId);
                _protocol.StartProtocolSession(mSessionId);
            }
        }
    }

    @Override
    public void onTransportDisconnected(String info) {
        if (!_isHeartbeatTimedout) {
            // Pass directly to connection listener
            _connectionListener.onTransportDisconnected(info);
        }
        _isHeartbeatTimedout = false;
    }

    @Override
    public void onTransportError(String info, Exception e) {
        if (!_isHeartbeatTimedout) {
            // Pass directly to connection listener
            _connectionListener.onTransportError(info, e);
        }
        _isHeartbeatTimedout = false;
    }

    @Override
    public void onServerSocketInit(int serverSocketPort) {
        Log.d("SyncConnection", "ServerSocket init: " + serverSocketPort);
        mNSDHelper.registerService(serverSocketPort);
        mNSDHelper.discoverServices();
    }

    @Override
    public void onProtocolMessageBytesToSend(byte[] msgBytes, int offset,
                                             int length) {
        // Protocol has packaged bytes to send, pass to transport for transmission
        synchronized (TRANSPORT_REFERENCE_LOCK) {
            if (_transport != null) {
                //Log.d(TAG, "<- Bytes:" + BitConverter.bytesToHex(msgBytes));
                _transport.sendBytes(msgBytes, offset, length);
            }
        }
    }

    @Override
    public void onProtocolMessageReceived(ProtocolMessage msg) {
        _connectionListener.onProtocolMessageReceived(msg);

        // Unblock USB reader thread by this method
        FunctionID functionID = new FunctionID();
        String functionName = functionID.getFunctionName(msg.getFunctionID());
        if (functionName != null && functionName.equals(Names.OnAppInterfaceUnregistered)) {
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
                    if (_transport != null) {
                        _transport.stopReading();
                    }
                }
            }
        }
    }

    @Override
    public void onProtocolSessionStarted(Session session,
                                         byte version, String correlationID) {

        _connectionListener.onProtocolSessionStarted(session, version, correlationID);
    }

    @Override
    public void onSecureServiceStarted(byte version) {
        _connectionListener.onSecureServiceStarted(version);
    }

    @Override
    public void onProtocolServiceEnded(ServiceType serviceType, byte sessionID,
                                       String correlationID) {
        _connectionListener.onProtocolServiceEnded(serviceType, sessionID, correlationID);
        processEndService(serviceType);
    }

    private void processEndService(ServiceType serviceType) {
        if (_transport != null && serviceType.equals(ServiceType.RPC)) {
            synchronized (END_PROTOCOL_SERVICE_RPC_LOCK) {
                END_PROTOCOL_SERVICE_RPC_LOCK.notifyAll();
            }
            _transport.stopReading();
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
        if (_heartbeatMonitor != null) {
            _heartbeatMonitor.heartbeatACKReceived();
        }
    }

    @Override
    public void onResetHeartbeat(){
        if (_heartbeatMonitor != null) {
            _heartbeatMonitor.notifyTransportActivity();
        }
    }

    @Override
    public void onProtocolError(String info, Exception e) {
        _connectionListener.onProtocolError(info, e);
    }

    @Override
    public void onProtocolAppUnregistered() {
        Log.d(TAG, "onProtocolAppUnregistered");
    }

    @Override
    public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, byte version,
                                         String correlationID) {
        _connectionListener.onProtocolServiceStarted(serviceType, sessionID, version, correlationID);
    }

    @Override
    public void onMobileNavAckReceived(int frameReceivedNumber) {
        _connectionListener.onMobileNavAckReceived(frameReceivedNumber);
    }

    @Override
    public void onStartServiceNackReceived(ServiceType serviceType) {
        _connectionListener.onStartServiceNackReceived(serviceType);

        /*if (serviceType == ServiceType.Secure_Service) {
            startProtocolSession();
        }*/
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
        if (pm != null) {
            //_connectionListener.onPacketCreated(pm);
            sendMessage(pm);
        }
    }

    @Override
    public void sendHeartbeat(IHeartbeatMonitor monitor) {
        Log.d(TAG, "Asked to send heartbeat");
        final ProtocolFrameHeader heartbeat =
                ProtocolFrameHeaderFactory.createHeartbeat(ServiceType.Heartbeat,
                        (byte) 2);
        final byte[] bytes = heartbeat.assembleHeaderBytes();
        onProtocolMessageBytesToSend(bytes, 0, bytes.length);
    }

    @Override
    public void heartbeatTimedOut(IHeartbeatMonitor monitor) {
        Log.d(TAG, "Heartbeat timeout; closing connection");
        _isHeartbeatTimedout = true;
        closeConnection((byte) 0, false, false);
        _connectionListener.onHeartbeatTimedOut();
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
        Log.d(TAG, "SetSessionId:" + mSessionId);
    }
}