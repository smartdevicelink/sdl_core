package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;

public class SetDisplayLayoutResponse extends RPCResponse {

    public SetDisplayLayoutResponse() {
        super("SetDisplayLayout");
    }
    public SetDisplayLayoutResponse(Hashtable hash) {
        super(hash);
    }

    public DisplayCapabilities getDisplayCapabilities() {
        Object obj = parameters.get(Names.displayCapabilities);
        if (obj instanceof DisplayCapabilities) {
            return (DisplayCapabilities) obj;
        } else if (obj instanceof Hashtable) {
            return new DisplayCapabilities((Hashtable) obj);
        }
        return null;
    }

    public void setDisplayCapabilities(DisplayCapabilities displayCapabilities) {
        if (displayCapabilities != null) {
            parameters.put(Names.displayCapabilities, displayCapabilities);
        } else {
            parameters.remove(Names.displayCapabilities);
        }
    }
}