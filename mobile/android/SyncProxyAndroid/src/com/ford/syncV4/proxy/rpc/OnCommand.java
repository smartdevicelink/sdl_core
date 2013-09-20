package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.TriggerSource;
import com.ford.syncV4.util.DebugTool;

public class OnCommand extends RPCNotification {

    public OnCommand() {
        super("OnCommand");
    }
    public OnCommand(Hashtable hash) {
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
    public TriggerSource getTriggerSource() {
        Object obj = parameters.get(Names.triggerSource);
        if (obj instanceof TriggerSource) {
            return (TriggerSource) obj;
        } else if (obj instanceof String) {
            TriggerSource theCode = null;
            try {
                theCode = TriggerSource.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.triggerSource, e);
            }
            return theCode;
        }
        return null;
    }
    public void setTriggerSource( TriggerSource triggerSource ) {
        if (triggerSource != null) {
            parameters.put(Names.triggerSource, triggerSource );
        }
    }
}
