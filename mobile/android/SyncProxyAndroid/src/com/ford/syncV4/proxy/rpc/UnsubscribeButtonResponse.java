package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class UnsubscribeButtonResponse extends RPCResponse {

    public UnsubscribeButtonResponse() {
        super("UnsubscribeButton");
    }
    public UnsubscribeButtonResponse(Hashtable hash) {
        super(hash);
    }
}