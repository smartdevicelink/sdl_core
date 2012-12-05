package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class SubscribeButtonResponse extends RPCResponse {

    public SubscribeButtonResponse() {
        super("SubscribeButton");
    }
    public SubscribeButtonResponse(Hashtable hash) {
        super(hash);
    }
}