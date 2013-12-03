package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class CreateInteractionChoiceSet extends RPCRequest {

    public CreateInteractionChoiceSet() {
        super("CreateInteractionChoiceSet");
    }
    public CreateInteractionChoiceSet(Hashtable hash) {
        super(hash);
    }
    public Integer getInteractionChoiceSetID() {
        return (Integer) parameters.get( Names.interactionChoiceSetID );
    }
    public void setInteractionChoiceSetID( Integer interactionChoiceSetID ) {
        if (interactionChoiceSetID != null) {
            parameters.put(Names.interactionChoiceSetID, interactionChoiceSetID );
        }
    }
    public Vector<Choice> getChoiceSet() {
        if (parameters.get(Names.choiceSet) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.choiceSet);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof Choice) {
	                return (Vector<Choice>) list;
	            } else if (obj instanceof Hashtable) {
	                Vector<Choice> newList = new Vector<Choice>();
	                for (Object hashObj : list) {
	                    newList.add(new Choice((Hashtable)hashObj));
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
    public void setChoiceSet( Vector<Choice> choiceSet ) {
        if (choiceSet != null) {
            parameters.put(Names.choiceSet, choiceSet );
        }
    }
}
