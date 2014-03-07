package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.constants.Names;

public class OnAudioPassThru extends RPCNotification {

    public OnAudioPassThru() {
        super("OnAudioPassThru");
    }
    public OnAudioPassThru(Hashtable hash) {
        super(hash);
    }
    public void setAPTData(byte[] aptData) {
        if (aptData != null) {
            store.put(Names.bulkData, aptData);
        } else {
        	store.remove(Names.bulkData);
        }
    }
    public byte[] getAPTData() {
        return (byte[]) store.get(Names.bulkData);
    }
}
