package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.TriggerSource;
import com.ford.syncV4.util.DebugTool;

public class PerformInteractionResponse extends RPCResponse {

    public PerformInteractionResponse() {
        super("PerformInteraction");
    }
    public PerformInteractionResponse(Hashtable hash) {
        super(hash);
    }
    public Integer getChoiceID() {
        return (Integer) parameters.get( Names.choiceID );
    }
    public void setChoiceID( Integer choiceID ) {
        if (choiceID != null) {
            parameters.put(Names.choiceID, choiceID );
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
    public void setManualTextEntry(String manualTextEntry) {
        if (manualTextEntry != null) {
            parameters.put(Names.manualTextEntry, manualTextEntry);
        } else {
            parameters.remove(Names.manualTextEntry);
        }
    }
    public String getManualTextEntry() {
        return (String) parameters.get(Names.manualTextEntry);
    }
}
