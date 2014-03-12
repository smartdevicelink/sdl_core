package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;

import java.util.Hashtable;
import java.util.Vector;

public class SetDisplayLayoutResponse extends RPCResponse {

    public SetDisplayLayoutResponse() {
        super("SetDisplayLayout");
    }

    public SetDisplayLayoutResponse(Hashtable hash) {
        super(hash);
    }

    public DisplayCapabilities getDisplayCapabilities() {
        Object obj = parameters.get(Names.displayCapabilities);
        if (obj instanceof DisplayCapabilities) {
            return (DisplayCapabilities) obj;
        } else if (obj instanceof Hashtable) {
            return new DisplayCapabilities((Hashtable) obj);
        }
        return null;
    }

    public void setDisplayCapabilities(
            DisplayCapabilities displayCapabilities) {
        if (displayCapabilities != null) {
            parameters.put(Names.displayCapabilities, displayCapabilities);
        } else {
            parameters.remove(Names.displayCapabilities);
        }
    }

    public Vector<ButtonCapabilities> getButtonCapabilities() {
        final Object o = parameters.get(Names.buttonCapabilities);
        if (o instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) o;
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof ButtonCapabilities) {
                    return (Vector<ButtonCapabilities>) list;
                } else if (obj instanceof Hashtable) {
                    Vector<ButtonCapabilities> newList =
                            new Vector<ButtonCapabilities>();
                    for (Object hashObj : list) {
                        newList.add(
                                new ButtonCapabilities((Hashtable) hashObj));
                    }
                    return newList;
                }
            }
        }
        return null;
    }

    public void setButtonCapabilities(
            Vector<ButtonCapabilities> buttonCapabilities) {
        if (buttonCapabilities != null) {
            parameters.put(Names.buttonCapabilities, buttonCapabilities);
        } else {
            parameters.remove(Names.buttonCapabilities);
        }
    }

    public Vector<SoftButtonCapabilities> getSoftButtonCapabilities() {
        final Object o = parameters.get(Names.softButtonCapabilities);
        if (o instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) o;
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof SoftButtonCapabilities) {
                    return (Vector<SoftButtonCapabilities>) list;
                } else if (obj instanceof Hashtable) {
                    Vector<SoftButtonCapabilities> newList =
                            new Vector<SoftButtonCapabilities>();
                    for (Object hashObj : list) {
                        newList.add(new SoftButtonCapabilities(
                                (Hashtable) hashObj));
                    }
                    return newList;
                }
            }
        }
        return null;
    }

    public void setSoftButtonCapabilities(
            Vector<SoftButtonCapabilities> softButtonCapabilities) {
        if (softButtonCapabilities != null) {
            parameters.put(Names.softButtonCapabilities,
                    softButtonCapabilities);
        } else {
            parameters.remove(Names.softButtonCapabilities);
        }
    }

    public PresetBankCapabilities getPresetBankCapabilities() {
        Object obj = parameters.get(Names.presetBankCapabilities);
        if (obj instanceof PresetBankCapabilities) {
            return (PresetBankCapabilities) obj;
        } else if (obj instanceof Hashtable) {
            return new PresetBankCapabilities((Hashtable) obj);
        }
        return null;
    }

    public void setPresetBankCapabilities(
            PresetBankCapabilities presetBankCapabilities) {
        if (presetBankCapabilities != null) {
            parameters.put(Names.presetBankCapabilities,
                    presetBankCapabilities);
        } else {
            parameters.remove(Names.presetBankCapabilities);
        }
    }
}