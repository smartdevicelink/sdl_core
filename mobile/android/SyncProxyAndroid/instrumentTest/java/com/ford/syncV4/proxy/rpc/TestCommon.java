package com.ford.syncV4.proxy.rpc;

import android.test.InstrumentationTestCase;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Common methods for testing classes.
 *
 * Created by enikolsky on 2013-10-29.
 */
public class TestCommon {
    public static JSONObject paramsToRequestObject(JSONObject paramsObject)
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        JSONObject requestObject = new JSONObject();
        jsonObject.put("request", requestObject);
        requestObject.put("parameters", paramsObject);
        return jsonObject;
    }

    public static void setupMocking(InstrumentationTestCase testCase) {
        System.setProperty("dexmaker.dexcache", testCase.getInstrumentation()
                                                        .getTargetContext()
                                                        .getCacheDir()
                                                        .getPath());
    }
}
