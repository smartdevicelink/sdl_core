package com.ford.syncV4.proxy;

import com.ford.syncV4.proxy.rpc.SyncMsgVersion;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 24.06.14
 * Time: 13:45
 */
public class RPCStructFactory {

    /**
     * Create instance of the {@link com.ford.syncV4.proxy.rpc.SyncMsgVersion} object with actual
     * {@link com.ford.syncV4.proxy.rpc.SyncMsgVersion#getMajorVersion()} and
     * {@link com.ford.syncV4.proxy.rpc.SyncMsgVersion#getMinorVersion()} fields values
     *
     * @return instance of the {@link com.ford.syncV4.proxy.rpc.SyncMsgVersion}
     */
    public static SyncMsgVersion createSyncMsgVersion() {
        SyncMsgVersion syncMsgVersion = new SyncMsgVersion();
        syncMsgVersion.setMajorVersion(3);
        syncMsgVersion.setMinorVersion(0);
        return syncMsgVersion;
    }
}