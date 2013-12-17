package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class SetMediaClockTimerResponse extends RPCResponse {

    public SetMediaClockTimerResponse() {
        super("SetMediaClockTimer");
    }
    public SetMediaClockTimerResponse(Hashtable hash) {
        super(hash);
    }
}