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

	// Called to indicate that a protocol syncSession has been started (from either side)
	void onProtocolSessionStarted(byte sessionId, byte version);

	// Called to indicate that a protocol syncSession has ended (from either side)
	void onProtocolServiceEnded(ServiceType serviceType, byte sessionID);

    /**
     * Called when a protocol heartbeat ACK message has been received from SYNC.
     */
    void onProtocolHeartbeatACK(byte sessionId);

    void onProtocolHeartbeat(byte sessionId);

    void onResetHeartbeatAck(byte sessionId);

	// Called to indicate that a protocol error was detected in received data.
	void onProtocolError(String info, Exception e);

    void onMobileNavAckReceived(byte sessionId, int frameReceivedNumber);

    void onProtocolAppUnregistered();

    void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, byte version);

    void onStartServiceNackReceived(byte sessionId, ServiceType serviceType);

    void onResetHeartbeat(byte sessionId);
}