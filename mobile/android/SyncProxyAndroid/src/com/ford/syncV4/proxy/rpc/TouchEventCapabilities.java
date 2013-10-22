package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;

import java.util.Hashtable;

public class TouchEventCapabilities extends RPCStruct {
    public TouchEventCapabilities() {}
    
    public TouchEventCapabilities(Hashtable hash) {
        super(hash);
    }
    
    public void setPressAvailable(Boolean pressAvailable) {
        if (pressAvailable != null) {
            store.put(Names.pressAvailable, pressAvailable);
        } else {
        	store.remove(Names.pressAvailable);
        }
    }
    
    public Boolean getPressAvailable() {
        return (Boolean) store.get(Names.pressAvailable);
    }
    
    public void setMultiTouchAvailable(Boolean multiTouchAvailable) {
        if (multiTouchAvailable != null) {
            store.put(Names.multiTouchAvailable, multiTouchAvailable);
        } else {
        	store.remove(Names.multiTouchAvailable);
        }
    }
    
    public Boolean getMultiTouchAvailable() {
        return (Boolean) store.get(Names.multiTouchAvailable);
    }
    
    public void setDoublePressAvailable(Boolean doublePressAvailable) {
        if (doublePressAvailable != null) {
            store.put(Names.doublePressAvailable, doublePressAvailable);
        } else {
        	store.remove(Names.doublePressAvailable);
        }
    }
    
    public Boolean getDoublePressAvailable() {
        return (Boolean) store.get(Names.doublePressAvailable);
    }
}