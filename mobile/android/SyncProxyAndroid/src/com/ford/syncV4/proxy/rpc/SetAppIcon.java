package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class SetAppIcon extends RPCRequest {

    public SetAppIcon() {
        super("SetAppIcon");
    }
    public SetAppIcon(Hashtable hash) {
        super(hash);
    }
    public void setSyncFileName(String syncFileName) {
        if (syncFileName != null) {
            parameters.put(Names.syncFileName, syncFileName);
        } else {
        	parameters.remove(Names.syncFileName);
        }
    }
    public String getSyncFileName() {
        return (String) parameters.get(Names.syncFileName);
    }
}
