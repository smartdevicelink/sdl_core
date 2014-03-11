package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

import java.util.Hashtable;
import java.util.Vector;

public class DiagnosticMessage extends RPCRequest {
    public DiagnosticMessage() {
        super("DiagnosticMessage");
    }

    public DiagnosticMessage(Hashtable hash) {
        super(hash);
    }

    public Integer getTargetID() {
        final Object o = parameters.get(Names.targetID);
        if (o instanceof Integer) {
            return (Integer) o;
        }
        return null;
    }

    public void setTargetID(Integer targetID) {
        if (targetID != null) {
            parameters.put(Names.targetID, targetID);
        } else {
            parameters.remove(Names.targetID);
        }
    }

    public Integer getMessageLength() {
        final Object o = parameters.get(Names.messageLength);
        if (o instanceof Integer) {
            return (Integer) o;
        }
        return null;
    }

    public void setMessageLength(Integer messageLength) {
        if (messageLength != null) {
            parameters.put(Names.messageLength, messageLength);
        } else {
            parameters.remove(Names.messageLength);
        }
    }

    public Vector<Integer> getMessageData() {
        final Object o = parameters.get(Names.messageData);
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

    public void setMessageData(Vector<Integer> messageData) {
        if (messageData != null) {
            parameters.put(Names.messageData, messageData);
        } else {
            parameters.remove(Names.messageData);
        }
    }
}
