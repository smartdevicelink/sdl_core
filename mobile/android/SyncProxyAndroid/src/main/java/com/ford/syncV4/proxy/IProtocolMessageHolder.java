package com.ford.syncV4.proxy;

import com.ford.syncV4.protocol.ProtocolMessage;

import java.util.List;

/**
 * Stores pending protocol messages for specified correlation ID.
 *
 * Created by enikolsky on 2014-01-22.
 */
public interface IProtocolMessageHolder {
    /**
     * Saves the ProtocolMessage objects. All the messages in the list are
     * expected to have the same correlation ID.
     *
     * @param messages messages to save
     */
    void saveMessages(List<ProtocolMessage> messages);

    /**
     * Clears stored messages for the specified correlation ID.
     *
     * @param correlationID correlation ID
     */
    void clearMessages(Integer correlationID);

    /**
     * Checks if there are messages left for the correlation ID.
     *
     * @param correlationID correlation ID
     * @return true if there are messages left
     */
    boolean hasMessages(Integer correlationID);

    /**
     * Returns the next message for the correlation ID.
     *
     * @param correlationID correlation ID
     * @return next message in the sequence, or null if none
     */
    ProtocolMessage peekNextMessage(Integer correlationID);

    /**
     * Returns the next message for the correlation ID and removes it from the
     * sequence.
     *
     * @param correlationID correlation ID
     * @return next message in the sequence, or null if none
     */
    ProtocolMessage popNextMessage(Integer correlationID);
}
