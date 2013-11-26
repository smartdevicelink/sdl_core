package com.ford.avarsdl.requests;

import com.ford.avarsdl.service.SDLService;
import com.ford.avarsdl.util.Logger;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.rpc.CancelAccess;

import org.json.JSONObject;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11/26/13
 * Time: 11:41 AM
 */
public class CancelAccessCommand implements RequestCommand {

    @Override
    public void execute(int id, JSONObject jsonParameters) {
        SyncProxyALM proxy = SDLService.getProxyInstance();
        if (proxy != null) {
            CancelAccess msg = new CancelAccess();
            msg.setCorrelationID(id);
            try {
                proxy.sendRPCRequest(msg);
            } catch (Throwable e) {
                Logger.e(getClass().getSimpleName() + " can't send message", e);
            }
        }
    }
}