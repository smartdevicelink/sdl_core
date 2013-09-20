package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class AddSubMenu extends RPCRequest {

    public AddSubMenu() {
        super("AddSubMenu");
    }
    public AddSubMenu(Hashtable hash) {
        super(hash);
    }
    public Integer getMenuID() {
        return (Integer) parameters.get( Names.menuID );
    }
    public void setMenuID( Integer menuID ) {
        if (menuID != null) {
            parameters.put(Names.menuID, menuID );
        }
    }
    public Integer getPosition() {
        return (Integer) parameters.get( Names.position );
    }
    public void setPosition( Integer position ) {
        if (position != null) {
            parameters.put(Names.position, position );
        }
    }
    public String getMenuName() {
        return (String) parameters.get( Names.menuName );
    }
    public void setMenuName( String menuName ) {
        if (menuName != null) {
            parameters.put(Names.menuName, menuName );
        }
    }
}
