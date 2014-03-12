package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;

public class SoftButtonCapabilities extends RPCStruct {

    public SoftButtonCapabilities() { }
    public SoftButtonCapabilities(Hashtable hash) {
        super(hash);
    }
    public void setShortPressAvailable(Boolean shortPressAvailable) {
        if (shortPressAvailable != null) {
            store.put(Names.shortPressAvailable, shortPressAvailable);
        } else {
        	store.remove(Names.shortPressAvailable);
        }
    }
    public Boolean getShortPressAvailable() {
        return (Boolean) store.get( Names.shortPressAvailable);
    }
    public void setLongPressAvailable(Boolean longPressAvailable) {
        if (longPressAvailable != null) {
            store.put(Names.longPressAvailable, longPressAvailable);
        } else {
        	store.remove(Names.longPressAvailable);
        }
    }
    public Boolean getLongPressAvailable() {
        return (Boolean) store.get( Names.longPressAvailable);
    }
    public void setUpDownAvailable(Boolean upDownAvailable) {
        if (upDownAvailable != null) {
            store.put(Names.upDownAvailable, upDownAvailable);
        } else {
        	store.remove(Names.upDownAvailable);
        }
    }
    public Boolean getUpDownAvailable() {
        return (Boolean) store.get( Names.upDownAvailable);
    }
    public void setImageSupported(Boolean imageSupported) {
        if (imageSupported != null) {
            store.put(Names.imageSupported, imageSupported);
        } else {
        	store.remove(Names.imageSupported);
        }
    }
    public Boolean getImageSupported() {
        return (Boolean) store.get( Names.imageSupported);
    }
}
