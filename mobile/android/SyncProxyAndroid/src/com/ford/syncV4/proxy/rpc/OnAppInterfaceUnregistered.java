package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.AppInterfaceUnregisteredReason;
import com.ford.syncV4.util.DebugTool;

public class OnAppInterfaceUnregistered extends RPCNotification {

    public OnAppInterfaceUnregistered() {
        super("OnAppInterfaceUnregistered");
    }
    public OnAppInterfaceUnregistered(Hashtable hash) {
        super(hash);
    }
    public AppInterfaceUnregisteredReason getReason() {
        Object obj = parameters.get(Names.reason);
        if (obj instanceof AppInterfaceUnregisteredReason) {
            return (AppInterfaceUnregisteredReason) obj;
        } else if (obj instanceof String) {
            AppInterfaceUnregisteredReason theCode = null;
            try {
                theCode = AppInterfaceUnregisteredReason.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.reason, e);
            }
            return theCode;
        }
        return null;
    }
    public void setReason( AppInterfaceUnregisteredReason reason ) {
        if (reason != null) {
            parameters.put(Names.reason, reason );
        }
    }
}
