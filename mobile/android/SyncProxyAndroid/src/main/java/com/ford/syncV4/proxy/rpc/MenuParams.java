package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;

public class MenuParams extends RPCStruct {

    public MenuParams() { }
    public MenuParams(Hashtable hash) {
        super(hash);
    }
    public Integer getParentID() {
        final Object o = store.get(Names.parentID);
        if (o instanceof Integer) {
            return (Integer) o;
        }
        return null;
    }
    public void setParentID( Integer parentID ) {
        if (parentID != null) {
            store.put(Names.parentID, parentID );
        } else {
            store.remove(Names.parentID);
        }
    }
    public Integer getPosition() {
        final Object o = store.get(Names.position);
        if (o instanceof Integer) {
            return (Integer) o;
        }
        return null;
    }
    public void setPosition( Integer position ) {
        if (position != null) {
            store.put(Names.position, position );
        } else {
            store.remove(Names.position);
        }
    }
    public String getMenuName() {
        final Object o = store.get(Names.menuName);
        if (o instanceof String) {
            return (String) o;
        }
        return null;
    }
    public void setMenuName( String menuName ) {
        if (menuName != null) {
            store.put(Names.menuName, menuName );
        } else {
            store.remove(Names.menuName);
        }
    }
}
