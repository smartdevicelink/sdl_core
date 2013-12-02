package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class DeleteInteractionChoiceSet extends RPCRequest {

    public DeleteInteractionChoiceSet() {
        super("DeleteInteractionChoiceSet");
    }
    public DeleteInteractionChoiceSet(Hashtable hash) {
        super(hash);
    }
    public Integer getInteractionChoiceSetID() {
        return (Integer) parameters.get( Names.interactionChoiceSetID );
    }
    public void setInteractionChoiceSetID( Integer interactionChoiceSetID ) {
        if (interactionChoiceSetID != null) {
            parameters.put(Names.interactionChoiceSetID, interactionChoiceSetID );
        }
    }
}