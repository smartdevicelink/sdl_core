package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCMessage;
import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.constants.Names;

import java.util.Hashtable;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/25/14
 * Time: 1:42 PM
 */

/**
 * Notification containing an updated hashID which can be used over connection cycles
 * (i.e. loss of connection, ignition cycles, etc.). Sent after initial registration and
 * subsequently after any change in the calculated hash of all persisted app data.
 */
public class OnHashChange extends RPCNotification {

    private static final int HASH_ID_MAX_LENGTH = 100;

    public OnHashChange() {
        super(Names.OnHashChange);
    }

    public OnHashChange(Hashtable hash) {
        super(hash);
    }

    public OnHashChange(RPCMessage rpcMsg) {
        super(rpcMsg);
    }

    /**
     * Return calculated hash ID to be referenced during RegisterAppInterface.
     *
     * @return {@link java.lang.String} hash ID
     */
    public String getHashID() {
        if (!parameters.containsKey(Names.hashID)) {
            return null;
        }
        if (parameters.get(Names.hashID) instanceof String) {
            return (String) parameters.get(Names.hashID);
        }
        return null;
    }

    /**
     * Set calculated hash ID to be referenced during RegisterAppInterface.
     *
     * @param hashID hash ID
     */
    public void setHashID(String hashID) {
        if (hashID != null) {
            if (hashID.length() > HASH_ID_MAX_LENGTH) {
                throw new IllegalArgumentException("Parameter 'hashId' can have max length " +
                        "'" + HASH_ID_MAX_LENGTH + "', current length is '" + hashID.length() + "'");
            }
            parameters.put(Names.hashID, hashID);
        } else {
            parameters.remove(Names.hashID);
        }
    }
}