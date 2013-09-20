package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;

public class AlertResponse extends RPCResponse {

    public AlertResponse() {
        super("Alert");
    }
    public AlertResponse(Hashtable hash) {
        super(hash);
    }
    public Integer getTryAgainTime() {
        return (Integer) parameters.get(Names.tryAgainTime);
    }
    public void setTryAgainTime(Integer tryAgainTime) {
        if (tryAgainTime != null) {
            parameters.put(Names.tryAgainTime, tryAgainTime);
        } else {
        	parameters.remove(Names.tryAgainTime);
        }
    }
}