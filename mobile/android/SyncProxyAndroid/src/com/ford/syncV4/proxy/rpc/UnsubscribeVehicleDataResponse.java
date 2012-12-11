package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import com.ford.syncV4.proxy.RPCResponse;

public class UnsubscribeVehicleDataResponse extends RPCResponse {

    public UnsubscribeVehicleDataResponse() {
        super("UnsubscribeVehicleData");
    }
    public UnsubscribeVehicleDataResponse(Hashtable hash) {
        super(hash);
    }
}
