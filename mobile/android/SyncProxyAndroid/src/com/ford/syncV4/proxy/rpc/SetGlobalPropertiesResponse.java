package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class SetGlobalPropertiesResponse extends RPCResponse {

    public SetGlobalPropertiesResponse() {
        super("SetGlobalProperties");
    }
    public SetGlobalPropertiesResponse(Hashtable hash) {
        super(hash);
    }
}