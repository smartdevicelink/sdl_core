package com.ford.syncV4.messageDispatcher;

import java.util.Comparator;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.util.logger.Logger;

public class OutgoingProtocolMessageComparator implements Comparator<ProtocolMessage> {

    @SuppressWarnings("unused")
    private static final String CLASS_NAME = OutgoingProtocolMessageComparator.class.getSimpleName();

	@Override
	public int compare(ProtocolMessage arg0, ProtocolMessage arg1) {
		// Always return 0, turning the priority queue into a FIFO queue.

        //Logger.d(CLASS_NAME + " arg0:" + arg0.getCorrID() + " " + arg1.getCorrID());

        int result = 0;
        /*if (arg0.getServiceType().getValue() < arg1.getServiceType().getValue()) {
            result = 1;
        } else {
            result = -1;
        }

        if (arg0.getServiceType() == ServiceType.Mobile_Nav && arg1.getServiceType() == ServiceType.Mobile_Nav) {
            if (arg0.getCorrID() < arg1.getCorrID()) {
                result = 1;
            } else {
                result = -1;
            }
        }*/

		return result;
	}
}

