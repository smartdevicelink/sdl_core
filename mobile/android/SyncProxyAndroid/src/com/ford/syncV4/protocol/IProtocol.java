package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.SessionType;
import com.ford.syncV4.transport.ISyncTransport;
import com.ford.syncV4.transport.ITransportListener;

public interface IProtocol extends ITransportListener {

	void handleBytesFromTransport(byte[] receivedBytes, int receivedBytesLength);
	public void sendStartSession(SessionType sessionType);
	public void sendEndSession(SessionType sessionType, byte sessionID);
	
	public void sendData(SessionType sessionType, byte sessionID, byte[] data);
	
	public ISyncTransport getTransport();
	public void setTransport(ISyncTransport transport);
	
	void addProtocolListener(IProtocolListener listener);
	boolean removeProtocolListener(IProtocolListener listener);
	
	public void close();
}
