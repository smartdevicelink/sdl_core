package com.ford.syncV4.transport;

import com.ford.syncV4.exception.SyncException;

public interface ISyncTransport {
	
	void connect() throws SyncException;

	void disconnect();

	boolean sendBytes(byte[] message);
	boolean sendBytes(byte[] message, int offset, int length);

	void addTransportListener(ITransportListener transListener);
	
	String getGuid();	
	String getAppName();	
	void setAppName(String appName);
}
