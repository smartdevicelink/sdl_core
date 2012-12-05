package com.ford.syncV4.transport;

public interface ITransportListener {
	// Called to indicate and deliver bytes received from transport
	void onTransportBytesReceived(byte[] receivedBytes, int receivedBytesLength);

	// Called to indicate that transport connection was established
	void onTransportConnected();

	// Called to indicate that transport was disconnected (by either side)
	void onTransportDisconnected(String info);

	// Called to indicate that some error occurred on the transport
	void onTransportError(String info, Exception e);
} // end-interface