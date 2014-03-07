package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class PerformAudioPassThruResponse extends RPCResponse {

    public PerformAudioPassThruResponse() {
        super("PerformAudioPassThru");
    }
    public PerformAudioPassThruResponse(Hashtable hash) {
        super(hash);
    }
}