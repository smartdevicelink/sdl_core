/**
 * 
 */
package com.ford.syncV4.proxy;

import java.util.Hashtable;

import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.util.DebugTool;

public class RPCResponse extends RPCMessage {
	
	public RPCResponse(String functionName) {
		super(functionName, "response");
	}

	public RPCResponse(Hashtable hash) {
		super(hash);
	}

	public RPCResponse(RPCMessage rpcMsg) {
		super(rpcMsg);
	}

	public Integer getCorrelationID() {
		return (Integer)function.get(Names.correlationID);
	}
	
	public void setCorrelationID(Integer correlationID) {
		if (correlationID != null) {
            function.put(Names.correlationID, correlationID );
        } else if (parameters.contains(Names.correlationID)){
        	function.remove(Names.correlationID);
        }
	}
	
	public Boolean getSuccess() {
        return (Boolean) parameters.get( Names.success );
    }
	
    public void setSuccess( Boolean success ) {
        if (success != null) {
            parameters.put(Names.success, success );
        }
    }
    
    public Result getResultCode() {
        Object obj = parameters.get(Names.resultCode);
        if (obj instanceof Result) {
            return (Result) obj;
        } else if (obj instanceof String) {
            Result theCode = null;
            try {
                theCode = Result.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.resultCode, e);
            }
            return theCode;
        }
        return null;
    }
    
    public void setResultCode( Result resultCode ) {
        if (resultCode != null) {
            parameters.put(Names.resultCode, resultCode );
        }
    }
    
    public String getInfo() {
        return (String) parameters.get( Names.info );
    }
    
    public void setInfo( String info ) {
        if (info != null) {
            parameters.put(Names.info, info );
        }
    }
}
