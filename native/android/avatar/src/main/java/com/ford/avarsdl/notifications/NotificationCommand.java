package com.ford.avarsdl.notifications;

import com.ford.syncV4.proxy.RPCNotification;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11/19/13
 * Time: 5:19 PM
 */
public interface NotificationCommand {
    public void execute(String method, RPCNotification notification);
}