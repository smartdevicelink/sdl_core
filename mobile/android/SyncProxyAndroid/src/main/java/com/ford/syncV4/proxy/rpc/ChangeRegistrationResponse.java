package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;

public class ChangeRegistrationResponse extends RPCResponse {

    public ChangeRegistrationResponse() {
        super("ChangeLanguageRegistration");
    }
    public ChangeRegistrationResponse(Hashtable hash) {
        super(hash);
    }
}