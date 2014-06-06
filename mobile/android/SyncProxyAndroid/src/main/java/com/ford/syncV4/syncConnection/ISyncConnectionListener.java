package com.ford.syncV4.syncConnection;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.ServiceType;

public interface ISyncConnectionListener {

    /**
     * Calling this method indicates that the connection via selected transport is established
     */
    public void onTransportConnected();

	public void onTransportDisconnected(String info);
	
	public void onTransportError(String info, Exception e);

    public void onHeartbeatTimedOut(byte sessionId);
	
	public void onProtocolMessageReceived(ProtocolMessage msg);
	
<<<<<<< HEAD
	public void onProtocolSessionStarted(Session session, byte version, String correlationID);

	public void onProtocolServiceEnded(ServiceType serviceType,
                                       byte sessionID, String correlationID);
=======
	public void onProtocolSessionStarted(byte sessionId, byte version);
	
	public void onProtocolServiceEnded(ServiceType serviceType, byte sessionId);

    public void onProtocolServiceEndedAck(ServiceType serviceType, byte sessionId);
>>>>>>> develop
	
	public void onProtocolError(String info, Throwable e);

    public void onMobileNavAckReceived(byte sessionId, int frameReceivedNumber);

<<<<<<< HEAD
    public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, boolean encrypted, byte version, String correlationID);
=======
    public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, byte version);
>>>>>>> develop

    public void onStartServiceNackReceived(byte sessionId, ServiceType serviceType);

    /**
     * Sends {@link com.ford.syncV4.protocol.ProtocolMessage} to the
     * {@link com.ford.syncV4.messageDispatcher.ProxyMessageDispatcher} queue
     *
     * @param protocolMessage {@link com.ford.syncV4.protocol.ProtocolMessage}
     */
    public void sendOutgoingMessage(ProtocolMessage protocolMessage);
}