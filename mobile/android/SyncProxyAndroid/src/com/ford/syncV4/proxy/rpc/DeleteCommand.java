package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class DeleteCommand extends RPCRequest {

    public DeleteCommand() {
        super("DeleteCommand");
    }
    public DeleteCommand(Hashtable hash) {
        super(hash);
    }
    public Integer getCmdID() {
        return (Integer) parameters.get( Names.cmdID );
    }
    public void setCmdID( Integer cmdID ) {
        if (cmdID != null) {
            parameters.put(Names.cmdID, cmdID );
        }
    }
}