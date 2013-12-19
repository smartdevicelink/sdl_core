package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class DeleteCommandResponse extends RPCResponse {

    public DeleteCommandResponse() {
        super("DeleteCommand");
    }
    public DeleteCommandResponse(Hashtable hash) {
        super(hash);
    }
}