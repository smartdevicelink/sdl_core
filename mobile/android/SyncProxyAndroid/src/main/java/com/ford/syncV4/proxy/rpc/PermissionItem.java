package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.util.DebugTool;

public class PermissionItem extends RPCStruct {

    public PermissionItem() { }
    public PermissionItem(Hashtable hash) {
        super(hash);
    }
    public String getRpcName() {
        return (String) store.get(Names.rpcName);
    }
    public void setRpcName(String rpcName) {
        if (rpcName != null) {
        	store.put(Names.rpcName, rpcName);
        } else {
        	store.remove(Names.rpcName);
        }
    }
    public HMIPermissions getHMIPermissions() {
    	Object obj = store.get(Names.hmiPermissions);
        if (obj instanceof HMIPermissions) {
            return (HMIPermissions) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new HMIPermissions((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.hmiPermissions, e);
            }
        }
        return null;
    }
    public void setHMIPermissions(HMIPermissions hmiPermissions) {
        if (hmiPermissions != null) {
        	store.put(Names.hmiPermissions, hmiPermissions);
        } else {
        	store.remove(Names.hmiPermissions);
        }
    }
    public ParameterPermissions getParameterPermissions() {
    	Object obj = store.get(Names.parameterPermissions);
        if (obj instanceof ParameterPermissions) {
            return (ParameterPermissions) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new ParameterPermissions((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.parameterPermissions, e);
            }
        }
        return null;
    }
    public void setParameterPermissions(ParameterPermissions parameterPermissions) {
        if (parameterPermissions != null) {
        	store.put(Names.parameterPermissions, parameterPermissions);
        } else {
        	store.remove(Names.parameterPermissions);
        }
    }
}
