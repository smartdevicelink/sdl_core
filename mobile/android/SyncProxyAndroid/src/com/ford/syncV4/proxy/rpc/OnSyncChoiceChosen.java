package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.SyncProxyALMManager.SyncChoice;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.TriggerSource;
import com.ford.syncV4.util.DebugTool;

public class OnSyncChoiceChosen extends RPCNotification {

	public OnSyncChoiceChosen() {
		super(Names.OnSyncChoiceChosen);
	}
	public OnSyncChoiceChosen(Hashtable hash){
		super(hash);
	}
    public SyncChoice getSyncChoice() {
    	return (SyncChoice) parameters.get(Names.syncChoice);
    }
    public void setSyncChoice(SyncChoice syncChoice) {
    	if (syncChoice != null) {
    		parameters.put(Names.syncChoice, syncChoice);
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
