package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class DeleteInteractionChoiceSetResponse extends RPCResponse {

    public DeleteInteractionChoiceSetResponse() {
        super("DeleteInteractionChoiceSet");
    }
    public DeleteInteractionChoiceSetResponse(Hashtable hash) {
        super(hash);
    }
}