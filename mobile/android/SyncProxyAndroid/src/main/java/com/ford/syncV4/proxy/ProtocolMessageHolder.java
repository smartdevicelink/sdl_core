package com.ford.syncV4.proxy;

import android.util.SparseArray;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.FunctionID;
import com.ford.syncV4.util.logger.Logger;

import java.util.List;

/**
 * Stores pending protocol messages for specified correlation ID using a sparse
 * array.
 *
 * This class is not thread-safe.
 *
 * Created by enikolsky on 2014-01-22.
 */
public class ProtocolMessageHolder implements IProtocolMessageHolder {

    @SuppressWarnings("unused")
    private static final String LOG_TAG = ProtocolMessageHolder.class.getSimpleName();

    private final SparseArray<List<ProtocolMessage>> array = new SparseArray<List<ProtocolMessage>>();

    @Override
    public void saveMessages(List<ProtocolMessage> messages) {
        final int corrId = messages.get(0).getCorrID();
        array.put(corrId, messages);
    }

    @Override
    public void clearMessages(Integer correlationID) {
        array.remove(correlationID);
    }

    @Override
    public boolean hasMessages(Integer correlationID) {
        final List<ProtocolMessage> messages = array.get(correlationID);
        return ((null != messages) && (messages.size() > 0));
    }

    @Override
    public ProtocolMessage peekNextMessage(Integer correlationID) {
        if (hasMessages(correlationID)) {
            final List<ProtocolMessage> messages = array.get(correlationID);
            return messages.get(0);
        }

        return null;
    }

    @Override
    public ProtocolMessage popNextMessage(Integer correlationID) {
        if (hasMessages(correlationID)) {
            final List<ProtocolMessage> messages = array.get(correlationID);
            final ProtocolMessage pm = messages.get(0);

            messages.remove(0);
            if (0 == messages.size()) {
                clearMessages(correlationID);
            }

            return pm;
        }

        return null;
    }
}
