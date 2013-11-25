package com.ford.avarsdl.requests;

import org.json.JSONObject;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11/19/13
 * Time: 3:33 PM
 */
public interface RequestCommand {
    /**
     * Executes a request command
     * @param id - id of the message (this id is received from web part of the mobile app)
     * @param jsonParameters - JSON object that represents RPCStruct
     */
    public void execute(int id, JSONObject jsonParameters);
}