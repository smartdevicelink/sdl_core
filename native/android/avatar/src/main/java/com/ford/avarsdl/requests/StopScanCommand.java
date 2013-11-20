package com.ford.avarsdl.requests;

import com.ford.avarsdl.business.MainApp;
import com.ford.avarsdl.service.SDLService;
import com.ford.avarsdl.util.Logger;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.rpc.StopScan;

import org.json.JSONObject;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11/19/13
 * Time: 3:41 PM
 */
public class StopScanCommand implements RequestCommand {

    @Override
    public void execute(JSONObject jsonParameters) {
        SyncProxyALM proxy = SDLService.getProxyInstance();
        if (proxy != null) {
            StopScan msg = new StopScan();
            msg.setCorrelationID(MainApp.getInstance().nextCorrelationID());

            try {
                proxy.sendRPCRequest(msg);
            } catch (SyncException e) {
                Logger.e(getClass().getSimpleName() + " can't send message", e);
            }
        } else {
            Logger.e(getClass().getSimpleName() + " proxy is NULL");
        }
    }
}