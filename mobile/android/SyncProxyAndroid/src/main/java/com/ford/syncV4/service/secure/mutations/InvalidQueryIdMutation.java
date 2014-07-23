package com.ford.syncV4.service.secure.mutations;

import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolMessage;

/**
 * Created by admin on 7/2/14.
 */
public class InvalidQueryIdMutation  extends AbstractMutation{
    private static final int INVALID_FUNCTION_ID = 0xDEAD;

    @Override
    protected void mutateMessage(ProtocolFrameHeader header, byte[] dataChunk) {

    }

    @Override
    protected ProtocolMessage mutateMessage(ProtocolMessage protocolMessage) {
        protocolMessage.setFunctionID(INVALID_FUNCTION_ID);
        return protocolMessage;
    }
}
