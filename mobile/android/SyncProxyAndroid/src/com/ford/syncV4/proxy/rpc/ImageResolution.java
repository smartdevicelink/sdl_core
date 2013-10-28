package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;

import java.util.Hashtable;

public class ImageResolution extends RPCStruct {
    public ImageResolution() {}
    
    public ImageResolution(Hashtable hash) {
        super(hash);
    }
    
    public void setResolutionWidth(Integer resolutionWidth) {
        if (resolutionWidth != null) {
            store.put(Names.resolutionWidth, resolutionWidth);
        } else {
        	store.remove(Names.resolutionWidth);
        }
    }
    
    public Integer getResolutionWidth() {
        return (Integer) store.get(Names.resolutionWidth);
    }
    
    public void setResolutionHeight(Integer resolutionHeight) {
        if (resolutionHeight != null) {
            store.put(Names.resolutionHeight, resolutionHeight);
        } else {
        	store.remove(Names.resolutionHeight);
        }
    }
    
    public Integer getResolutionHeight() {
        return (Integer) store.get(Names.resolutionHeight);
    }
}