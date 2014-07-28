package com.ford.syncV4.streaming;

import com.ford.syncV4.protocol.ProtocolMessage;

/**
 * Interface that provides a methods for the {@link com.ford.syncV4.streaming.AbstractPacketizer}'s
 * implementers to inform about actions to perform over provided stream
 */
public interface IStreamListener {

    /**
     * Method which allows to send {@link com.ford.syncV4.protocol.ProtocolMessage} over stream
     *
     * @param protocolMessage {@link com.ford.syncV4.protocol.ProtocolMessage} as a result of the
     *                        packetizing
     */
	void sendH264(ProtocolMessage protocolMessage);
}