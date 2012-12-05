package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class PutFile extends RPCRequest {

	public PutFile(String functionName) {
		super(functionName);
		// TODO Auto-generated constructor stub
	}

	public PutFile(Hashtable hash) {
		super(hash);
		// TODO Auto-generated constructor stub
	}
	
	
	public String getSyncFileName() {
        return (String) parameters.get( "syncFileName" );
    }
    public void setSyncFileName( String syncFileName ) {
        if (syncFileName != null) {
            parameters.put("syncFileName", syncFileName );
        }
    }
    
    

}
