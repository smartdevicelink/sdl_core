package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCResponse;

import java.util.Hashtable;

public class SystemRequestResponse extends RPCResponse {
    public SystemRequestResponse() {
        super("SystemRequest");
    }

    public SystemRequestResponse(Hashtable hash) {
        super(hash);
    }
}