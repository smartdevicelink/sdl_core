package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;

public interface IProtocolListener {

	// Called to indicate that these bytes are to be sent as part of a message.
	// This call includes the part of the message.
	void onProtocolMessageBytesToSend(byte[] msgBytes, int offset, int length);

	// Called to indicate that a complete message (RPC, BULK, etc.) has been
	// received.  This call includes the message.
	void onProtocolMessageReceived(ProtocolMessage msg);

	// Called to indicate that a protocol currentSession has been started (from either side)
	void onProtocolSessionStarted(Session session, byte version, String correlationID);

	// Called to indicate that a protocol currentSession has ended (from either side)
	void onProtocolServiceEnded(ServiceType serviceType, byte sessionID, String correlationID /*, String info, Exception ex*/);

    /**
     * Called when a protocol heartbeat ACK message has been received from SYNC.
     */
    void onProtocolHeartbeatACK();

    void onResetHeartbeat();

	// Called to indicate that a protocol error was detected in received data.
	void onProtocolError(String info, Exception e);

    void onMobileNavAckReceived(int frameReceivedNumber);

    void onProtocolAppUnregistered();

    void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, byte version, String correlationID);

    void onStartServiceNackReceived(ServiceType serviceType);
}