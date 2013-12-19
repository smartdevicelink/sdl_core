package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCRequest;

public class UnregisterAppInterface extends RPCRequest {

    public UnregisterAppInterface() {
        super("UnregisterAppInterface");
    }
    public UnregisterAppInterface(Hashtable hash) {
        super(hash);
    }
}