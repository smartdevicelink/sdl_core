package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class AlertResponse extends RPCResponse {

    public AlertResponse() {
        super("Alert");
    }
    public AlertResponse(Hashtable hash) {
        super(hash);
    }
}