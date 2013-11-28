package com.ford.avarsdl.jsoncontroller;

import com.ford.avarsdl.views.AvatarActivity;
import com.ford.avarsdl.util.Logger;
import com.ford.avarsdl.util.RPCConst;

public class JSONAVAController extends JSONController {

    private String mJSComponentName = null;
    private AvatarActivity mActivity;

    public JSONAVAController(AvatarActivity activity, String cname) {
        super(RPCConst.CN_AVATAR);
        mActivity = activity;
        mJSComponentName = cname;
    }

    public JSONAVAController(String cname, ITcpClient client) {
        super(RPCConst.CN_AVATAR, client);
        mJSComponentName = cname;
    }

    protected void processRequest(String request) {
        processNotification(request);
    }

    protected String processNotification(String notification) {
        Logger.d(getClass().getSimpleName() + " Process notification");
        mJSONParser.putJSONObject(notification);
        final String func = "FFW.WebSocketSimulator.receive('" + mJSComponentName
                + "','" + notification + "')";
        Logger.d(getClass().getSimpleName() + " Send notification to JS");
        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mActivity.getWebView().loadUrl("javascript:" + func);
            }
        });
        return null;
    }

    protected void processResponse(String response) {
        if (!processRegistrationResponse(response)) {
            final String func = "FFW.WebSocketSimulator.receive('" + mJSComponentName
                    + "','" + response + "')";
            Logger.d(getClass().getSimpleName() + " : " + func);
            mActivity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mActivity.getWebView().loadUrl("javascript:" + func);
                }
            });
        }
    }

    public void sendJSMessage(String cName, String jsonMsg) {
        Logger.d(getClass().getSimpleName() + " SendJSMessage : " + jsonMsg);
        mJSONParser.putJSONObject(jsonMsg);
        if (mJSONParser.getId() >= 0 &&
                mJSONParser.getResult() == null &&
                mJSONParser.getError() == null) {
            mJSComponentName = cName;
        }
        jsonMsg += System.getProperty("line.separator");
        sendJSONMsg(jsonMsg);
    }
}