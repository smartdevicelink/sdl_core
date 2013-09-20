package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class DeleteSubMenuResponse extends RPCResponse {

    public DeleteSubMenuResponse() {
        super("DeleteSubMenu");
    }
    public DeleteSubMenuResponse(Hashtable hash) {
        super(hash);
    }
}