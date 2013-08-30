package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.constants.Names;

public class OnSyncPData extends RPCNotification {
	public OnSyncPData() {
        super("OnSyncPData");
    }
    public OnSyncPData(Hashtable hash) {
        super(hash);
    }
    public String getUrl(){
        return (String) parameters.get(Names.URL);
    }
    public void setUrl(String url) {
    	if (url != null) {
    		parameters.put(Names.URL, url);
    	} else {
			parameters.remove(Names.URL);
        }
    }
    public Integer getTimeout(){
    	if (parameters.get(Names.Timeout) instanceof Integer) {
    		return (Integer)parameters.get(Names.Timeout);
    	}
    	return null;
    }
    public void setTimeout(Integer timeout) {
        if (timeout != null) {
            parameters.put(Names.timeout, timeout);
        } else {
        	parameters.remove(Names.timeout);
        }
    }
    public void setSyncPData(byte[] syncPData) {
        if (syncPData != null) {
            store.put(Names.bulkData, syncPData);
        } else {
        	store.remove(Names.bulkData);
        }
    }
    public byte[] getSyncPData() {
        return (byte[]) store.get(Names.bulkData);
    }
}
