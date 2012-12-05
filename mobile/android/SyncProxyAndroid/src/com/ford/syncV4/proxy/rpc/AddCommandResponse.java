package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class AddCommandResponse extends RPCResponse {

    public AddCommandResponse() {
        super("AddCommand");
    }
    public AddCommandResponse(Hashtable hash) {
        super(hash);
    }
}

