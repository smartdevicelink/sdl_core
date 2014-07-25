package com.ford.syncV4.service.secure.mutations;

import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolMessage;

/**
 * Created by admin on 7/3/14.
 */
public class InvalidPayloadMutation extends AbstractMutation {
    @Override
    protected void mutateMessage(ProtocolFrameHeader header, byte[] dataChunk) {
        for (int i = 0; i < dataChunk.length; i++) {
            dataChunk[i] = 0xb;
        }
    }

    @Override
    protected ProtocolMessage mutateMessage(ProtocolMessage protocolMessage) {
        return protocolMessage;
    }
}
