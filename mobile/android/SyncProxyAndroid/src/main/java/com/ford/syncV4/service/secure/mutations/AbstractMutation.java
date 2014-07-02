package com.ford.syncV4.service.secure.mutations;

import com.ford.syncV4.protocol.ProtocolMessage;

/**
 * Created by admin on 7/2/14.
 */
public abstract class AbstractMutation implements ISecureServiceMutation {

    protected static final int MUTATION_SIZE_FACTOR = 13;
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

    protected abstract ProtocolMessage mutateMessage(ProtocolMessage protocolMessage);

    public void setLastMutation(AbstractMutation mutation) {
        if (nextMutation == null) {
            nextMutation = mutation;
        } else {
            nextMutation.setLastMutation(mutation);
        }
    }
}
