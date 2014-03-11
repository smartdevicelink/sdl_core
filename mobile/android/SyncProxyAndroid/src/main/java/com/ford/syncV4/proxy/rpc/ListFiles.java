package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCRequest;

public class ListFiles extends RPCRequest {

    public ListFiles() {
        super("ListFiles");
    }
    public ListFiles(Hashtable hash) {
        super(hash);
    }
}