package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class ResetGlobalPropertiesResponse extends RPCResponse {

    public ResetGlobalPropertiesResponse() {
        super("ResetGlobalProperties");
    }
    public ResetGlobalPropertiesResponse(Hashtable hash) {
        super(hash);
    }
}