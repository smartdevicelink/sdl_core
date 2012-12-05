package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.constants.Names;

public class OnPermissionsChange extends RPCNotification {

	public OnPermissionsChange() {
		super("OnPermissionsChange");
	}
	public OnPermissionsChange(Hashtable hash) {
		super(hash);
	}

	public Vector<PermissionGroupItem> getPermissionGroupItems() {
		Vector<?> list = (Vector<?>)parameters.get(Names.PermissionGroupItems);
		if (list != null && list.size()>0) {
			Object obj = list.get(0);
			if(obj instanceof PermissionGroupItem){
				return (Vector<PermissionGroupItem>) list;
			} else if(obj instanceof Hashtable) {
				Vector<PermissionGroupItem> newList = new Vector<PermissionGroupItem>();
				for (Object hash:list) {
					newList.add(new PermissionGroupItem((Hashtable)hash));
				}
				return newList;
			}
		}
		return null;
	}
	public void setPermissionGroupItems(Vector<PermissionGroupItem> permissionGroupItems) {
		if (permissionGroupItems != null) {
			parameters.put(Names.PermissionGroupItems, permissionGroupItems);
		}
	}
}
