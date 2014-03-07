package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class ScrollableMessageResponse extends RPCResponse {

    public ScrollableMessageResponse() {
        super("ScrollableMessage");
    }
    public ScrollableMessageResponse(Hashtable hash) {
        super(hash);
    }
}