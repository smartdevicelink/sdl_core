package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.ButtonName;
import com.ford.syncV4.util.DebugTool;

public class ButtonCapabilities extends RPCStruct {

    public ButtonCapabilities() { }
    public ButtonCapabilities(Hashtable hash) {
        super(hash);
    }
    public ButtonName getName() {
        Object obj = store.get(Names.name);
        if (obj instanceof ButtonName) {
            return (ButtonName) obj;
        } else if (obj instanceof String) {
            ButtonName theCode = null;
            try {
                theCode = ButtonName.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.name, e);
            }
            return theCode;
        }
        return null;
    }
    public void setName( ButtonName name ) {
        if (name != null) {
            store.put(Names.name, name );
        }
    }
    public Boolean getShortPressAvailable() {
        return (Boolean) store.get( Names.shortPressAvailable );
    }
    public void setShortPressAvailable( Boolean shortPressAvailable ) {
        if (shortPressAvailable != null) {
            store.put(Names.shortPressAvailable, shortPressAvailable );
        }
    }
    public Boolean getLongPressAvailable() {
        return (Boolean) store.get( Names.longPressAvailable );
    }
    public void setLongPressAvailable( Boolean longPressAvailable ) {
        if (longPressAvailable != null) {
            store.put(Names.longPressAvailable, longPressAvailable );
        }
    }
    public Boolean getUpDownAvailable() {
        return (Boolean) store.get( Names.upDownAvailable );
    }
    public void setUpDownAvailable( Boolean upDownAvailable ) {
        if (upDownAvailable != null) {
            store.put(Names.upDownAvailable, upDownAvailable );
        }
    }
}
