package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class AddCommand extends RPCRequest {

    public AddCommand() {
        super("AddCommand");
    }
    public AddCommand(Hashtable hash) {
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
    public MenuParams getMenuParams() {
        Object obj = parameters.get(Names.menuParams);
        if (obj instanceof MenuParams) {
        	return (MenuParams)obj;
        }
        else if (obj instanceof Hashtable) {
        	return new MenuParams((Hashtable)obj);
        }
        return null;
    }
    public void setMenuParams( MenuParams menuParams ) {
        if (menuParams != null) {
            parameters.put(Names.menuParams, menuParams );
        }
    }
    public Vector<String> getVrCommands() {
    	if (parameters.get(Names.vrCommands) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.vrCommands);
	    	if (list != null && list.size() > 0) {
	    		Object obj = list.get(0);
	    		if (obj instanceof String) {
	    			return (Vector<String>)list;
	    		}
	    	}
    	}
    	return null;
    }
    public void setVrCommands( Vector<String> vrCommands ) {
        if (vrCommands != null) {
            parameters.put(Names.vrCommands, vrCommands );
        }
    }
}
