package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class EndAudioPassThruResponse extends RPCResponse {

    public EndAudioPassThruResponse() {
        super("EndAudioPassThru");
    }
    public EndAudioPassThruResponse(Hashtable hash) {
        super(hash);
    }
}