package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.util.DebugTool;

public class SetDisplayLayout extends RPCRequest {

    public SetDisplayLayout() {
        super("SetDisplayLayout");
    }
    public SetDisplayLayout(Hashtable hash) {
        super(hash);
    }
    public void setDisplayLayout(String displayLayout) {
        if (displayLayout != null) {
            parameters.put(Names.displayLayout, displayLayout);
        } else {
        	parameters.remove(Names.displayLayout);
        }
    }
    public String getDisplayLayout() {
    	return (String) parameters.get(Names.displayLayout);
    }
}
