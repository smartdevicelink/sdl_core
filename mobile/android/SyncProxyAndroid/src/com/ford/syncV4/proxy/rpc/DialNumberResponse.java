package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class DialNumberResponse extends RPCResponse {

    public DialNumberResponse() {
        super("DialNumber");
    }
    public DialNumberResponse(Hashtable hash) {
        super(hash);
    }
}