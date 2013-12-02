package com.ford.avarsdl.requests;

import com.ford.avarsdl.service.SDLService;
import com.ford.avarsdl.util.Logger;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.RadioStation;
import com.ford.syncV4.proxy.rpc.TuneRadio;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11/19/13
 * Time: 3:52 PM
 */
public class TuneRadioCommand implements RequestCommand {

    @Override
    public void execute(int id, JSONObject jsonParameters) {
        SyncProxyALM proxy = SDLService.getProxyInstance();
        if (proxy != null) {
            TuneRadio msg = new TuneRadio();
            msg.setCorrelationID(id);

            RadioStation radioStation = new RadioStation();
            if (jsonParameters != null) {
                try {
                    radioStation.deserializeJSON(jsonParameters.getJSONObject(Names.radioStation));
                    Logger.d(getClass().getSimpleName() + " RadioStation: " +
                            radioStation.serializeJSON().toString());
                } catch (JSONException e) {
                    Logger.e(getClass().getSimpleName() + " can not deserialize RadioStation: " + e);
                }
            }
            msg.setRadioStation(radioStation);

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