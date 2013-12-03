package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class UnregisterAppInterfaceResponse extends RPCResponse {

    public UnregisterAppInterfaceResponse() {
        super("UnregisterAppInterface");
    }
    public UnregisterAppInterfaceResponse(Hashtable hash) {
        super(hash);
    }
}