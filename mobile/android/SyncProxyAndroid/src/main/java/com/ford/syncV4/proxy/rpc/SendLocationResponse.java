package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCMessage;
import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;

import java.util.Hashtable;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 07.08.14
 * Time: 14:58
 */
public class SendLocationResponse extends RPCResponse {

    public SendLocationResponse() {
        super(Names.SendLocation);
    }

    public SendLocationResponse(Hashtable hash) {
        super(hash);
    }

    public SendLocationResponse(RPCMessage rpcMsg) {
        super(rpcMsg);
    }
}