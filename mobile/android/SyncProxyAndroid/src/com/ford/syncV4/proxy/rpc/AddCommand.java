package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.util.DebugTool;

public class AddCommand extends RPCRequest {

    public AddCommand() {
        super("AddCommand");
    }
    public AddCommand(Hashtable hash) {
        super(hash);
    }
    public Integer getCmdID() {
        return (Integer) parameters.get(Names.cmdID);
    }
    public void setCmdID(Integer cmdID) {
        if (cmdID != null) {
            parameters.put(Names.cmdID, cmdID);
        } else {
        	parameters.remove(Names.cmdID);
        }
    }
    public MenuParams getMenuParams() {
        Object obj = parameters.get(Names.menuParams);
        if (obj instanceof MenuParams) {
        	return (MenuParams) obj;
        }
        else if (obj instanceof Hashtable) {
        	try {
        		return new MenuParams((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.menuParams, e);
            }
        }
        return null;
    }
    public void setMenuParams(MenuParams menuParams) {
        if (menuParams != null) {
            parameters.put(Names.menuParams, menuParams);
        } else {
        	parameters.remove(Names.menuParams);
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
        } else {
        	parameters.remove(Names.vrCommands);
        }
    }
    public Image getCmdIcon() {
    	Object obj = parameters.get(Names.cmdIcon);
        if (obj instanceof Image) {
            return (Image) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new Image((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.cmdIcon, e);
            }
        }
        return null;
    }
    public void setCmdIcon(Image cmdIcon) {
        if (cmdIcon != null) {
            parameters.put(Names.cmdIcon, cmdIcon);
        } else {
        	parameters.remove(Names.cmdIcon);
        }
    }
}
