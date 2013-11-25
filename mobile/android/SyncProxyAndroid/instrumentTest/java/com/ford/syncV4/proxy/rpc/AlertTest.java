package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.marshal.JsonRPCMarshaller;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by enikolsky on 2013-10-23.
 */
public class AlertTest extends TestCase {
    private static final String PROGRESS_INDICATOR = "progressIndicator";

    public void testSerialization() throws JSONException {
        Alert msg = new Alert();
        assertNotNull(msg);

        final boolean progressIndicator = true;

        msg.setProgressIndicator(progressIndicator);

        JSONObject jsonObject = msg.serializeJSON((byte) 2);
        assertEquals(1, jsonObject.length());
        assertEquals(progressIndicator,
                jsonObject.getBoolean(PROGRESS_INDICATOR));
    }

    public void testNullDeserialization() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        Alert msg = new Alert(JsonRPCMarshaller
                .deserializeJSONObject(paramsToRequestObject(jsonObject)));
        assertNotNull(msg);
        assertNull(msg.getProgressIndicator());
    }

    public void testProgressIndicatorAPI() {
        Alert msg = new Alert();

        final Boolean progressIndicator = true;
        msg.setProgressIndicator(progressIndicator);

        assertEquals(progressIndicator, msg.getProgressIndicator());
    }

    public void testRemoveProgressIndicator() {
        Alert msg = new Alert();
        msg.setProgressIndicator(true);
        msg.setProgressIndicator(null);
        assertNull(msg.getProgressIndicator());
    }

    public void testGetProgressIndicator() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Boolean progressIndicator = true;
        jsonObject.put(PROGRESS_INDICATOR, progressIndicator);

        Alert msg = new Alert(JsonRPCMarshaller
                .deserializeJSONObject(paramsToRequestObject(jsonObject)));
        assertNotNull(msg);
        assertEquals(progressIndicator, msg.getProgressIndicator());
    }

    public void testGetProgressIndicatorIncorrect() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(PROGRESS_INDICATOR, 42);

        Alert msg = new Alert(JsonRPCMarshaller
                .deserializeJSONObject(paramsToRequestObject(jsonObject)));
        assertNotNull(msg);
        assertNull(msg.getProgressIndicator());
    }

    private JSONObject paramsToRequestObject(JSONObject paramsObject)
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        JSONObject requestObject = new JSONObject();
        jsonObject.put("request", requestObject);
        requestObject.put("parameters", paramsObject);
        return jsonObject;
    }
}
