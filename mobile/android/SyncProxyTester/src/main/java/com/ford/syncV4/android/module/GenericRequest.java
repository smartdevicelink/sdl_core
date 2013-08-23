package com.ford.syncV4.android.module;

import com.ford.syncV4.proxy.RPCRequest;

import java.util.Hashtable;

/**
 * A custom {@link com.ford.syncV4.proxy.RPCRequest} subclass that is not
 * present in the specification. Used to test that the response is
 * {@link com.ford.syncV4.proxy.rpc.GenericResponse}.
 */
public class GenericRequest extends RPCRequest {
    public static final String NAME = GenericRequest.class.getSimpleName();

    public GenericRequest() {
        super(NAME);
    }

    public GenericRequest(Hashtable hash) {
        super(hash);
    }
}
