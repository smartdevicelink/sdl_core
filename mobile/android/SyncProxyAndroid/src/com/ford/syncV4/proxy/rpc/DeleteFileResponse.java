package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;

public class DeleteFileResponse extends RPCResponse {

    public DeleteFileResponse() {
        super("DeleteFile");
    }
    public DeleteFileResponse(Hashtable hash) {
        super(hash);
    }
    public void setSpaceAvailable(Integer spaceAvailable) {
        if (spaceAvailable != null) {
            parameters.put(Names.spaceAvailable, spaceAvailable);
        } else {
        	parameters.remove(Names.spaceAvailable);
        }
    }
    public Integer getSpaceAvailable() {
        return (Integer) parameters.get(Names.spaceAvailable);
    }
}