package com.ford.avarsdl.requests;

import com.ford.avarsdl.service.SDLService;
import com.ford.avarsdl.util.Logger;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.rpc.TuneUp;

import org.json.JSONObject;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11/28/13
 * Time: 12:24 PM
 */
public class TuneUpCommand implements RequestCommand {

    @Override
    public void execute(int id, JSONObject jsonParameters) {
        SyncProxyALM proxy = SDLService.getProxyInstance();
        if (proxy != null) {
            TuneUp msg = new TuneUp();
            msg.setCorrelationID(id);

            try {
                //Logger.d(getClass().getSimpleName() + "Sending Tune Up: " + jsonParameters);
                proxy.sendRPCRequest(msg);
            } catch (SyncException e) {
                Logger.e(getClass().getSimpleName() + " can't send message", e);
            }
        } else {
            Logger.e(getClass().getSimpleName() + " proxy is NULL");
        }
    }
}