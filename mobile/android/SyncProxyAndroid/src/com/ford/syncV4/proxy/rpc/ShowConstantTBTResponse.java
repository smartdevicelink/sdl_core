package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class ShowConstantTBTResponse extends RPCResponse {

    public ShowConstantTBTResponse() {
        super("ShowConstantTBT");
    }
    public ShowConstantTBTResponse(Hashtable hash) {
        super(hash);
    }
}