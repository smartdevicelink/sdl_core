package com.ford.syncV4.messageDispatcher;

import com.ford.syncV4.protocol.ProtocolMessage;

import java.util.Comparator;

public class OutgoingProtocolMessageComparator implements Comparator<ProtocolMessage> {

	@Override
	public int compare(ProtocolMessage arg0, ProtocolMessage arg1) {
		// Always return 0, turning the priority queue into a FIFO queue.
		return 0;
	}
}