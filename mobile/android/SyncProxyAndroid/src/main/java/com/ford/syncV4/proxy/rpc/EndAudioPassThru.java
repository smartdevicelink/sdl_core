package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCRequest;

public class EndAudioPassThru extends RPCRequest {

    public EndAudioPassThru() {
        super("EndAudioPassThru");
    }
    public EndAudioPassThru(Hashtable hash) {
        super(hash);
    }
}
