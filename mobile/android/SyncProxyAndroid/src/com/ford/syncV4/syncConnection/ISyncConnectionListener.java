package com.ford.syncV4.syncConnection;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.SessionType;

public interface ISyncConnectionListener {
	public void onTransportDisconnected(String info);
	
	public void onTransportError(String info, Exception e);
	
	public void onProtocolMessageReceived(ProtocolMessage msg);
	
	public void onProtocolSessionStarted(SessionType sessionType,
			byte sessionID, byte version, String correlationID);
	
	public void onProtocolSessionEnded(SessionType sessionType,
			byte sessionID, String correlationID);
	
	public void onProtocolError(String info, Exception e);

    public void onMobileNavAckReceived(int frameReceivedNumber);
}
