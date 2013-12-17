package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.TBTState;
import com.ford.syncV4.util.DebugTool;

public class OnTBTClientState extends RPCNotification {

    public OnTBTClientState() {
        super("OnTBTClientState");
    }
    public OnTBTClientState(Hashtable hash) {
        super(hash);
    }
    public TBTState getState() {
        Object obj = parameters.get(Names.state);
        if (obj instanceof TBTState) {
        	return (TBTState)obj;
        } else if(obj instanceof String) {
        	TBTState theCode = null;
        	try{
        		theCode = TBTState.valueForString((String) obj);
        	} catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.state, e);
            }
        	return theCode;
        }    	
    	return null;
    }
    public void setState( TBTState state ) {
        if (state != null) {
            parameters.put(Names.state, state );
        } else {
        	parameters.remove(Names.state);
        }
    }
} // end-class