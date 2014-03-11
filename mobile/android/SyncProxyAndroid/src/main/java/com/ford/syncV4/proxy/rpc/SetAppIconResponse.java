package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class SetAppIconResponse extends RPCResponse {

    public SetAppIconResponse() {
        super("SetAppIcon");
    }
    public SetAppIconResponse(Hashtable hash) {
        super(hash);
    }
}