package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;

import java.util.Hashtable;
import java.util.Vector;

public class DiagnosticMessageResponse extends RPCResponse {
    public DiagnosticMessageResponse() {
        super("DiagnosticMessage");
    }

    public DiagnosticMessageResponse(Hashtable hash) {
        super(hash);
    }

    public Vector<Integer> getMessageDataResult() {
        final Object o = parameters.get(Names.messageDataResult);
        if (o instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) o;
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof Integer) {
                    return (Vector<Integer>) list;
                }
            }
        }
        return null;
    }

    public void setMessageDataResult(Vector<Integer> messageDataResult) {
        if (messageDataResult != null) {
            parameters.put(Names.messageDataResult, messageDataResult);
        } else {
            parameters.remove(Names.messageDataResult);
        }
    }
}
