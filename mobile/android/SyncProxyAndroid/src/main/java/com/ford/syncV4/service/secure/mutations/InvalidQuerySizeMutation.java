package com.ford.syncV4.service.secure.mutations;

import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolMessage;

/**
 * Created by admin on 7/2/14.
 */
public class InvalidQuerySizeMutation extends AbstractMutation {

    @Override
    protected void mutateMessage(ProtocolFrameHeader header, byte[] dataChunk) {

    }

    @Override
    protected ProtocolMessage mutateMessage(ProtocolMessage protocolMessage) {
        protocolMessage.setData(new byte[0]);
        return protocolMessage;
    }
}
