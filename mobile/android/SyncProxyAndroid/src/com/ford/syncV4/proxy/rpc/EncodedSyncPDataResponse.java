package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class EncodedSyncPDataResponse  extends RPCResponse {
	public EncodedSyncPDataResponse() {
        super("EncodedSyncPData");
    }
    public EncodedSyncPDataResponse(Hashtable hash) {
        super(hash);
    }
}
