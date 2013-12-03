package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class SpeakResponse extends RPCResponse {

    public SpeakResponse() {
        super("Speak");
    }
    public SpeakResponse(Hashtable hash) {
        super(hash);
    }
}