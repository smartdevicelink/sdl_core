package com.ford.avarsdl.responses;

import com.ford.avarsdl.jsoncontroller.JSONController;
import com.ford.avarsdl.util.Logger;
import com.ford.avarsdl.util.RPCConst;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11/21/13
 * Time: 4:31 PM
 */
public class GrantAccessResponse extends JSONController implements ResponseCommand {

    public GrantAccessResponse() {
        super(RPCConst.CN_REVSDL);
    }

    @Override
    public void execute(int id, String result) {
        Logger.d(getClass().getSimpleName() + " id: " + id + ", result: " + result);
        sendResponse(id, result);
    }
}