package com.ford.syncV4.messageDispatcher;

import java.util.Comparator;

import com.ford.syncV4.protocol.ProtocolMessage;

public class IncomingProtocolMessageComparator implements Comparator<ProtocolMessage> {

	@Override
	public int compare(ProtocolMessage arg0, ProtocolMessage arg1) {
		// Always return 0, turning the priority queue into a FIFO queue. 

        int result;
        if (arg0.getServiceType().getValue() < arg1.getServiceType().getValue()) {
            result = -1;
        } else {
            result = 1;
        }

        return result;
	}
}
