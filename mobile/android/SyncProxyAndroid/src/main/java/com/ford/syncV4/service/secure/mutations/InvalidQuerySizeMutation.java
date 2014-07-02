package com.ford.syncV4.service.secure.mutations;

import com.ford.syncV4.protocol.ProtocolMessage;

/**
 * Created by admin on 7/2/14.
 */
public class InvalidQuerySizeMutation extends AbstractMutation {

    @Override
    protected ProtocolMessage mutateMessage(ProtocolMessage protocolMessage) {
        protocolMessage.setJsonSize(protocolMessage.getJsonSize() + MUTATION_SIZE_FACTOR);
        return protocolMessage;
    }
}
