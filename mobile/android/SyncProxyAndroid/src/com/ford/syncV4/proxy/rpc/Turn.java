package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;

public class Turn extends RPCStruct {

	public Turn() { }
    public Turn(Hashtable hash) {
        super(hash);
    }
    public void setNavigationText(String navigationText) {
        if (navigationText != null) {
            store.put(Names.navigationText, navigationText);
        } else {
        	store.remove(Names.navigationText);
        }
    }
    public String getNavigationText() {
        return (String) store.get(Names.navigationText);
    }
    public void setTurnIcon(Image turnIcon) {
        if (turnIcon != null) {
        	store.put(Names.turnIcon, turnIcon);
        } else {
        	store.remove(Names.turnIcon);
        }
    }
    public Image getTurnIcon() {
    	Object obj = store.get(Names.turnIcon);
        if (obj instanceof Image) {
            return (Image) obj;
        } else {
        	return new Image((Hashtable) obj);
        }
    }
}
