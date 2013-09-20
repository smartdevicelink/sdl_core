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
	public Vector<PermissionItem> getPermissionItem() {
		Vector<?> list = (Vector<?>)parameters.get(Names.permissionItem);
		if (list != null && list.size()>0) {
			Object obj = list.get(0);
			if(obj instanceof PermissionItem){
				return (Vector<PermissionItem>) list;
			} else if(obj instanceof Hashtable) {
				Vector<PermissionItem> newList = new Vector<PermissionItem>();
				for (Object hash:list) {
					newList.add(new PermissionItem((Hashtable)hash));
				}
				return newList;
			}
		}
		return null;
	}
	public void setPermissionItem(Vector<PermissionItem> permissionItem) {
		if (permissionItem != null) {
			parameters.put(Names.permissionItem, permissionItem);
		} else {
			parameters.remove(Names.permissionItem);
        }
	}
}
