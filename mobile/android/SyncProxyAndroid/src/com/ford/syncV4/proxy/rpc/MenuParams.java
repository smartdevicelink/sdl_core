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
        return (Integer) store.get( Names.parentID );
    }
    public void setParentID( Integer parentID ) {
        if (parentID != null) {
            store.put(Names.parentID, parentID );
        }
    }
    public Integer getPosition() {
        return (Integer) store.get( Names.position );
    }
    public void setPosition( Integer position ) {
        if (position != null) {
            store.put(Names.position, position );
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
}
