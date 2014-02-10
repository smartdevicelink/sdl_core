package com.ford.syncV4.syncConnection;

import android.util.Log;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.protocol.AbstractProtocol;
import com.ford.syncV4.protocol.IProtocolListener;
import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolFrameHeaderFactory;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.protocol.heartbeat.IHeartbeatMonitor;
import com.ford.syncV4.protocol.heartbeat.IHeartbeatMonitorListener;
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

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class SyncConnection implements IProtocolListener, ITransportListener, IStreamListener,
        IHeartbeatMonitorListener {
    private static final String TAG = "SyncConnection";
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


    /**
     * Constructor.
     *
     * @param listener Sync connection listener.
     */
    public SyncConnection(ISyncConnectionListener listener) {
        _connectionListener = listener;
    }

    public void init(BaseTransportConfig transportConfig) {
        init(transportConfig, null);
    }

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
        }
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
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (_protocol != null) {
                // If transport is still connected, sent EndProtocolSessionMessage
                if (sendFinishMessages && (_transport != null) &&
                        _transport.getIsConnected()) {
                    _protocol.EndProtocolService(ServiceType.RPC, rpcSessionID);
                }

            } // end-if
        }

        waitForRpcEndServiceACK();

        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (!keepConnection) {
                _protocol = null;
            }
        }
        if (!keepConnection) {
            if (_heartbeatMonitor != null) {
                _heartbeatMonitor.stop();
                _heartbeatMonitor = null;
            }
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

    public void startTransport() throws SyncException {
        _transport.openConnection();
    }

    public Boolean getIsConnected() {

        // If _transport is null, then it can't be connected
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
    public void onTransportBytesReceived(byte[] receivedBytes,
                                         int receivedBytesLength) {
        // Send bytes to protocol to be interpreted
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (_protocol != null) {
                _protocol.HandleReceivedBytes(receivedBytes,
                        receivedBytesLength);
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
        }
        startProtocolSession();
    }

    private void startProtocolSession() {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (_protocol != null) {
                Log.d(TAG, "StartProtocolSession, id:" + mSessionId);
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
                _transport.sendBytes(msgBytes, offset, length);
            }
        }
    }

    @Override
    public void onProtocolMessageReceived(ProtocolMessage msg) {
        _connectionListener.onProtocolMessageReceived(msg);
    }

    @Override
    public void onProtocolSessionStarted(Session session,
                                         byte version, String correlationID) {

        _connectionListener.onProtocolSessionStarted(session, version, correlationID);
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
    public void onProtocolError(String info, Exception e) {
        _connectionListener.onProtocolError(info, e);
    }

    @Override
    public void onProtocolAppUnregistered() {
        Log.d(TAG, "onProtocolAppUnregistered");
    }

    @Override
    public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, byte version, String correlationID) {
        _connectionListener.onProtocolServiceStarted(serviceType, sessionID, version, correlationID);
    }

    @Override
    public void onMobileNavAckReceived(int frameReceivedNumber) {
        _connectionListener.onMobileNavAckReceived(frameReceivedNumber);
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
            sendMessage(pm);
        }
    }

    @Override
    public void sendHeartbeat(IHeartbeatMonitor monitor) {
        Log.d(TAG, "Asked to send heartbeat");
        final ProtocolFrameHeader heartbeat =
                ProtocolFrameHeaderFactory.createHeartbeat(ServiceType.RPC,
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