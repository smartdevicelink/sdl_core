package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class DialNumber extends RPCRequest {

    public DialNumber() {
        super("DialNumber");
    }
    public DialNumber(Hashtable hash) {
        super(hash);
    }
    public String getNumber() {
        return (String) parameters.get(Names.number);
    }
    public void setNumber(String number) {
        if (number != null) {
            parameters.put(Names.number, number);
        } else {
        	parameters.remove(Names.number);
        }
    }
}