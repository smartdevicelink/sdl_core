package com.ford.avarsdl.jsoncontroller;

import android.util.Log;

import com.ford.avarsdl.jsonparser.ERevSDLMethods;
import com.ford.avarsdl.util.Const;
import com.ford.avarsdl.util.RPCConst;

public class JSONRevSDLController extends JSONController {
    private static final boolean DEBUG = true;
    private static final String TAG =
            JSONRevSDLController.class.getSimpleName();
    private Delegate delegate;

    public JSONRevSDLController() {
        super(RPCConst.CN_REVSDL);
    }

    public Delegate getDelegate() {
        return delegate;
    }

    public void setDelegate(Delegate delegate) {
        this.delegate = delegate;
    }

    @Override
    protected void processResponse(String response) {
        logMsg("received response " + response);
        processRegistrationResponse(response);
    }

    @Override
    protected void processRequest(String request) {
        String method = mJSONParser.getMethod();
        method = method.substring(method.indexOf('.') + 1, method.length());
        switch (ERevSDLMethods.valueOf(method)) {
            case sendSDLAccessRequest:
                logMsg("Received access request");
                if (delegate != null) {
                    delegate.onSDLAccessRequested(this);
                }
                break;

            default:
                logMsg("Unknown request: " + method);
                break;
        }
    }

    private void logMsg(String msg) {
        if (DEBUG && Const.DEBUG) {
            Log.i(TAG, msg);
        }
    }

    public static interface Delegate {
        public abstract void onSDLAccessRequested(
                JSONRevSDLController controller);
    }
}
