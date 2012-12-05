package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;

public class PresetBankCapabilities extends RPCStruct {

    public PresetBankCapabilities() { }
    public PresetBankCapabilities(Hashtable hash) {
        super(hash);
    }
    public void setOnScreenPresetsAvailable(Boolean onScreenPresetsAvailable) {
    	if (onScreenPresetsAvailable != null) {
    		store.put(Names.OnScreenPresetsAvailable, onScreenPresetsAvailable);
    	} else {
    		store.remove(Names.OnScreenPresetsAvailable);
    	}
    }
    public Boolean onScreenPresetsAvailable() {
    	return (Boolean) store.get(Names.OnScreenPresetsAvailable);
    }
}
