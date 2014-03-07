package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class AlertManeuverResponse extends RPCResponse {

    public AlertManeuverResponse() {
        super("AlertManeuver");
    }
    public AlertManeuverResponse(Hashtable hash) {
        super(hash);
    }
}