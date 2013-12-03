package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;

public class SyncMsgVersion extends RPCStruct {

    public SyncMsgVersion() { }
    public SyncMsgVersion(Hashtable hash) {
        super(hash);
    }
    public Integer getMajorVersion() {
        return (Integer) store.get( Names.majorVersion );
    }
    public void setMajorVersion( Integer majorVersion ) {
        if (majorVersion != null) {
            store.put(Names.majorVersion, majorVersion );
        }
    }
    public Integer getMinorVersion() {
        return (Integer) store.get( Names.minorVersion );
    }
    public void setMinorVersion( Integer minorVersion ) {
        if (minorVersion != null) {
            store.put(Names.minorVersion, minorVersion );
        }
    }
}