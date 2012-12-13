package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class UpdateTurnList extends RPCRequest {

    public UpdateTurnList() {
        super("UpdateTurnList");
    }
    public UpdateTurnList(Hashtable hash) {
        super(hash);
    }
    public void setTurnList(Vector<Turn> turnList) {
        if (turnList != null) {
            parameters.put(Names.turnList, turnList);
        } else {
        	parameters.remove(Names.turnList);
        }
    }
    public Vector<Turn> getTurnList() {
        if (parameters.get(Names.turnList) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.turnList);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof Turn) {
	                return (Vector<Turn>) list;
	            } else if (obj instanceof Hashtable) {
	                Vector<Turn> newList = new Vector<Turn>();
	                for (Object hashObj : list) {
	                    newList.add(new Turn((Hashtable)hashObj));
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
    public void setSoftButtons(Vector<SoftButton> softButtons) {
        if (softButtons != null) {
            parameters.put(Names.softButtons, softButtons);
        } else {
        	parameters.remove(Names.softButtons);
        }
    }
    public Vector<SoftButton> getSoftButtons() {
        if (parameters.get(Names.softButtons) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.softButtons);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof SoftButton) {
	                return (Vector<SoftButton>) list;
	            } else if (obj instanceof Hashtable) {
	                Vector<SoftButton> newList = new Vector<SoftButton>();
	                for (Object hashObj : list) {
	                    newList.add(new SoftButton((Hashtable)hashObj));
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
}
