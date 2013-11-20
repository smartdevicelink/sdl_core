package com.ford.avarsdl.requests;

import com.ford.avarsdl.jsonparser.JSONParser;

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
     * @param jsonParameters - JSON object that represents RPCStruct
     */
    public void execute(JSONObject jsonParameters);
}