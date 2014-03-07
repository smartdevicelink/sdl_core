package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.util.DebugTool;

import java.util.Hashtable;

public class SystemRequest extends RPCRequest {
    public SystemRequest() {
        super("SystemRequest");
    }

    public SystemRequest(Hashtable hash) {
        super(hash);
    }

    public SystemRequest(SystemRequest that) {
        super(that);
    }

    public RequestType getRequestType() {
        Object obj = parameters.get(Names.requestType);
        if (obj instanceof RequestType) {
            return (RequestType) obj;
        } else if (obj instanceof String) {
            RequestType theCode = null;
            try {
                theCode = RequestType.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.requestType, e);
            }
            return theCode;
        }
        return null;
    }

    public void setRequestType(RequestType requestType) {
        if (requestType != null) {
            parameters.put(Names.requestType, requestType);
        } else {
            parameters.remove(Names.requestType);
        }
    }
}
