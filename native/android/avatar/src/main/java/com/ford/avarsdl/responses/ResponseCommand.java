package com.ford.avarsdl.responses;

import org.json.JSONObject;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11/21/13
 * Time: 4:30 PM
 */
public interface ResponseCommand {

    /**
     *
     * @param id
     * @param result
     */
    public void execute(int id, String result);
}