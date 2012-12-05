package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class CreateInteractionChoiceSetResponse extends RPCResponse {

    public CreateInteractionChoiceSetResponse() {
        super("CreateInteractionChoiceSet");
    }
    public CreateInteractionChoiceSetResponse(Hashtable hash) {
        super(hash);
    }
}