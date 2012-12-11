package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class SetDisplayLayoutResponse extends RPCResponse {

    public SetDisplayLayoutResponse() {
        super("SetDisplayLayout");
    }
    public SetDisplayLayoutResponse(Hashtable hash) {
        super(hash);
    }
}