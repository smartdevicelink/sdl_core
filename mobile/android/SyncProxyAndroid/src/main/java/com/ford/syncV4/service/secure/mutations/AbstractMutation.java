package com.ford.syncV4.service.secure.mutations;

import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolMessage;

/**
 * Created by admin on 7/2/14.
 */
public abstract class AbstractMutation implements ISecureServiceMutation {

    private AbstractMutation nextMutation;

    public AbstractMutation getNextMutation() {
        return nextMutation;
    }

    public void setNextMutation(AbstractMutation nextMutation) {
        this.nextMutation = nextMutation;
    }

    @Override
    public ProtocolMessage mutate(ProtocolMessage message) {
        ProtocolMessage mutatedMessage = mutateMessage(message);
        if (nextMutation == null) {
            return mutatedMessage;
        } else {
            return nextMutation.mutate(mutatedMessage);
        }
    }

    @Override
    public void mutate(ProtocolFrameHeader header, byte[] dataChunk){
        mutateMessage(header, dataChunk);
        if (nextMutation == null) {
            return;
        } else {
            nextMutation.mutate(header, dataChunk);
        }
    }

    protected abstract void mutateMessage(ProtocolFrameHeader header, byte[] dataChunk);

    protected abstract ProtocolMessage mutateMessage(ProtocolMessage protocolMessage);

    public void setLastMutation(AbstractMutation mutation) {
        if (nextMutation == null) {
            nextMutation = mutation;
        } else {
            nextMutation.setLastMutation(mutation);
        }
    }
}
