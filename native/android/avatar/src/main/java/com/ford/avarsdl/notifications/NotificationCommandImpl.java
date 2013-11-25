package com.ford.avarsdl.notifications;

import com.ford.avarsdl.jsoncontroller.JSONController;
import com.ford.avarsdl.util.Logger;
import com.ford.avarsdl.util.RPCConst;
import com.ford.syncV4.proxy.RPCNotification;

import org.json.JSONException;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11/19/13
 * Time: 5:20 PM
 */
public class NotificationCommandImpl extends JSONController implements NotificationCommand {

    public NotificationCommandImpl() {
        super(RPCConst.CN_REVSDL);
    }

    @Override
    public void execute(String method, RPCNotification notification) {
        try {
            byte serializeMethod = 2;
            String notificationString = notification.serializeJSON(serializeMethod).toString();
            //Logger.d(getClass().getSimpleName() + " send method: " + method);
            Logger.d(getClass().getSimpleName() + " send: " + notificationString);
            sendNotification(method, notificationString);
        } catch (JSONException e) {
            Logger.e(getClass().getSimpleName() + " execute", e);
        }
    }
}