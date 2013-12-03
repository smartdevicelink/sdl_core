package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;
/** ************************************************************************
/* class: GenericResponseResponse
 * Generic Response is sent, when the name of a received msg cannot be retrieved. Only used in case of an error.
 * Currently, only resultCode INVALID_DATA is used.
*/
public class GenericResponse extends RPCResponse {

    public GenericResponse() {
        super("GenericResponse");
    }
    public GenericResponse(Hashtable hash) {
        super(hash);
    }
}