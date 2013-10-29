package com.ford.syncV4.streaming;

import com.ford.syncV4.protocol.ProtocolMessage;

public interface IStreamListener {
	void sendH264(ProtocolMessage pm);
}