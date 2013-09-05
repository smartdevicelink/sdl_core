package com.ford.syncV4.syncConnection;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.protocol.AbstractProtocol;
import com.ford.syncV4.protocol.IProtocolListener;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.SessionType;
import com.ford.syncV4.transport.BTTransport;
import com.ford.syncV4.transport.BaseTransportConfig;
import com.ford.syncV4.transport.ITransportListener;
import com.ford.syncV4.transport.SyncTransport;
import com.ford.syncV4.transport.TCPTransport;
import com.ford.syncV4.transport.TCPTransportConfig;
import com.ford.syncV4.transport.TransportType;

public class SyncConnection implements IProtocolListener, ITransportListener {

    protected SyncTransport _transport = null;
    AbstractProtocol _protocol = null;
    ISyncConnectionListener _connectionListener = null;
    // Thread safety locks
    Object TRANSPORT_REFERENCE_LOCK = new Object();
    Object PROTOCOL_REFERENCE_LOCK = new Object();

    /**
     * Constructor.
     *
     * @param listener        Sync connection listener.
     * @param transportConfig Transport configuration for this connection.
     */
    public SyncConnection(ISyncConnectionListener listener, BaseTransportConfig transportConfig) {
        _connectionListener = listener;

        // Initialize the transport
        synchronized (TRANSPORT_REFERENCE_LOCK) {
            // Ensure transport is null
            if (_transport != null) {
                if (_transport.getIsConnected()) {
                    _transport.disconnect();
                }
                _transport = null;
            }

            if (transportConfig.getTransportType() == TransportType.BLUETOOTH) {
                _transport = new BTTransport(this);
            } else if (transportConfig.getTransportType() == TransportType.TCP) {
                _transport = new TCPTransport((TCPTransportConfig) transportConfig, this);
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

    public void closeConnection(byte rpcSessionID) {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (_protocol != null) {
                // If transport is still connected, sent EndProtocolSessionMessage
                if (_transport != null && _transport.getIsConnected()) {
                    _protocol.EndProtocolSession(SessionType.RPC, rpcSessionID);
                }
                _protocol = null;
            } // end-if
        }

        synchronized (TRANSPORT_REFERENCE_LOCK) {
            if (_transport != null) {
                _transport.disconnect();
            }
            _transport = null;
        }
    }

    public void closeMobileNavSession(byte mobileNavSessionId) {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (_protocol != null) {
                // If transport is still connected, sent EndProtocolSessionMessage
                if (_transport != null && _transport.getIsConnected()) {
                    _protocol.EndProtocolSession(SessionType.Mobile_Nav, mobileNavSessionId);
                }
            } // end-if
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
        _protocol.SendMessage(msg);
    }

    @Override
    public void onTransportBytesReceived(byte[] receivedBytes,
                                         int receivedBytesLength) {
        // Send bytes to protocol to be interpreted
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (_protocol != null) {
                _protocol.HandleReceivedBytes(receivedBytes, receivedBytesLength);
            }
        }
    }

    @Override
    public void onTransportConnected() {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (_protocol != null) {
                _protocol.StartProtocolSession(SessionType.RPC);
            }
        }
    }

    public void startMobileNavSession() {
        synchronized (PROTOCOL_REFERENCE_LOCK) {
            if (_protocol != null) {
                _protocol.StartProtocolSession(SessionType.Mobile_Nav);
            }
        }
    }

    @Override
    public void onTransportDisconnected(String info) {
        // Pass directly to connection listener
        _connectionListener.onTransportDisconnected(info);
    }

    @Override
    public void onTransportError(String info, Exception e) {
        // Pass directly to connection listener
        _connectionListener.onTransportError(info, e);
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
    public void onProtocolSessionStarted(SessionType sessionType,
                                         byte sessionID, byte version, String correlationID) {
        _connectionListener.onProtocolSessionStarted(sessionType, sessionID, version, correlationID);
    }

    @Override
    public void onProtocolSessionEnded(SessionType sessionType, byte sessionID,
                                       String correlationID) {
        _connectionListener.onProtocolSessionEnded(sessionType, sessionID, correlationID);
    }

    @Override
    public void onProtocolHeartbeatPastDue(int heartbeatInterval_ms,
                                           int pastDue_ms) {
        // Not implemented on SYNC
    }

    @Override
    public void onProtocolError(String info, Exception e) {
        _connectionListener.onProtocolError(info, e);
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
}
