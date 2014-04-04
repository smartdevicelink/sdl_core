package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.util.logger.Logger;

import java.util.Hashtable;
import java.util.Vector;

public class ReadDIDResponse extends RPCResponse {
    public ReadDIDResponse() {
        super("ReadDID");
    }

    public ReadDIDResponse(Hashtable hash) {
        super(hash);
    }

    public Vector<DIDResult> getDidResult() {
        final Object o = parameters.get(Names.didResult);
        if (o instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) o;
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof DIDResult) {
                    return (Vector<DIDResult>) list;
                } else if (obj instanceof Hashtable) {
                    Vector<DIDResult> newList = new Vector<DIDResult>();
                    for (Object hashObj : list) {
                        DIDResult toAdd = null;
                        try {
                            toAdd = new DIDResult((Hashtable) hashObj);
                        } catch (Exception e) {
                            Logger.e("Failed to parse " +
                                    getClass().getSimpleName() + "." +
                                    Names.didResult, e);
                        }
                        if (toAdd != null) {
                            newList.add(toAdd);
                        }
                    }
                    return newList;
                }
            }
        }
        return null;
    }

    public void setDidResult(Vector<DIDResult> dataResult) {
        if (dataResult != null) {
            parameters.put(Names.didResult, dataResult);
        } else {
            parameters.remove(Names.didResult);
        }
    }
}