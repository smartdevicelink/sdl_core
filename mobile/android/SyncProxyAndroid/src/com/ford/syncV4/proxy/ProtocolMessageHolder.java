package com.ford.syncV4.proxy;

import android.util.SparseArray;

import com.ford.syncV4.protocol.ProtocolMessage;

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
    private SparseArray<List<ProtocolMessage>> array =
            new SparseArray<List<ProtocolMessage>>();

    @Override
    public void saveMessages(List<ProtocolMessage> messages) {
        final int corrID = messages.get(0).getCorrID();
        array.put(corrID, messages);
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
