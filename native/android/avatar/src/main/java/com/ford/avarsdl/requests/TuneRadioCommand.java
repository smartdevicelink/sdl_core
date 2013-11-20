package com.ford.avarsdl.requests;

import com.ford.avarsdl.business.MainApp;
import com.ford.avarsdl.service.SDLService;
import com.ford.avarsdl.util.Logger;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.rpc.TuneRadio;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11/19/13
 * Time: 3:52 PM
 */
public class TuneRadioCommand implements RequestCommand {

    @Override
    public void execute() {
        SyncProxyALM proxy = SDLService.getProxyInstance();
        if (proxy != null) {
            TuneRadio msg = new TuneRadio();
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