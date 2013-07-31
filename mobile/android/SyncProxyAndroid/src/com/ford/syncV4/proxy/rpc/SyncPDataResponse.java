package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class SyncPDataResponse  extends RPCResponse {
	public SyncPDataResponse() {
        super("SyncPDataResponse");
    }
    public SyncPDataResponse(Hashtable hash) {
        super(hash);
    }
}