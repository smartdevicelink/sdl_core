package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;

public class Choice extends RPCStruct {

    public Choice() { }
    public Choice(Hashtable hash) {
        super(hash);
    }
    public Integer getChoiceID() {
        return (Integer) store.get( Names.choiceID );
    }
    public void setChoiceID( Integer choiceID ) {
        if (choiceID != null) {
            store.put(Names.choiceID, choiceID );
        }
    }
    public String getMenuName() {
        return (String) store.get( Names.menuName );
    }
    public void setMenuName( String menuName ) {
        if (menuName != null) {
            store.put(Names.menuName, menuName );
        }
    }
    public Vector<String> getVrCommands() {
        if (store.get(Names.vrCommands) instanceof Vector<?>) {
        	Vector<?> list = (Vector<?>)store.get( Names.vrCommands );
        	if (list != null && list.size() > 0) {
        		Object obj = list.get(0);
        		if (obj instanceof String) {
                	return (Vector<String>) list;        			
        		}
        	}
        }
        return null;
    }
    public void setVrCommands( Vector<String> vrCommands ) {
        if (vrCommands != null) {
            store.put(Names.vrCommands, vrCommands );
        }
    }
}
