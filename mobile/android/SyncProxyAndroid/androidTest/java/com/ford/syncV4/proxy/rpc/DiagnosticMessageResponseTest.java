package com.ford.syncV4.proxy.rpc;

import junit.framework.TestCase;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.Arrays;
import java.util.Vector;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for DiagnosticMessageResponse response.
 *
 * Created by enikolsky on 2014-02-17.
 */
public class DiagnosticMessageResponseTest extends TestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final String MESSAGE_DATA_RESULT = "messageDataResult";

    public void testMessageShouldBeCreated() {
        DiagnosticMessageResponse msg = new DiagnosticMessageResponse();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        DiagnosticMessageResponse msg = new DiagnosticMessageResponse();

        final Vector<Integer> messageDataResult =
                new Vector<Integer>(Arrays.asList(22, 11));

        msg.setMessageDataResult(messageDataResult);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        final JSONArray messageDataResultJsonArray =
                jsonObject.getJSONArray(MESSAGE_DATA_RESULT);
        assertThat(messageDataResultJsonArray.length(),
                is(messageDataResult.size()));
        assertThat(messageDataResultJsonArray.getInt(0),
                is(messageDataResult.get(0)));
        assertThat(messageDataResultJsonArray.getInt(1),
                is(messageDataResult.get(1)));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        DiagnosticMessageResponse msg = new DiagnosticMessageResponse(
                TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(msg, notNullValue());
        assertThat(msg.getMessageDataResult(), nullValue());
    }

    /// messageDataResult
    public void testGetMessageDataResultShouldReturnSetValue()
            throws JSONException {
        DiagnosticMessageResponse msg = new DiagnosticMessageResponse();

        final Vector<Integer> messageDataResult =
                new Vector<Integer>(Arrays.asList(22, 11));
        msg.setMessageDataResult(messageDataResult);

        assertThat(msg.getMessageDataResult(), is(messageDataResult));
    }

    public void testSettingNullMessageDataResultShouldRemoveValue()
            throws JSONException {
        DiagnosticMessageResponse msg = new DiagnosticMessageResponse();

        msg.setMessageDataResult(new Vector<Integer>());
        msg.setMessageDataResult(null);

        assertThat(msg.getMessageDataResult(), nullValue());
    }

    public void testDeserializedMessageDataResultAsVectorShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Vector<Integer> messageDataResult =
                new Vector<Integer>(Arrays.asList(22, 11));
        jsonObject.put(MESSAGE_DATA_RESULT, messageDataResult);

        DiagnosticMessageResponse msg = new DiagnosticMessageResponse(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getMessageDataResult(), is(messageDataResult));
    }

    public void testDeserializedMessageDataResultAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(MESSAGE_DATA_RESULT, 88);

        DiagnosticMessageResponse msg = new DiagnosticMessageResponse(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getMessageDataResult(), nullValue());
    }

    public void testDeserializedMessageDataResultAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(MESSAGE_DATA_RESULT, "88");

        DiagnosticMessageResponse msg = new DiagnosticMessageResponse(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getMessageDataResult(), nullValue());
    }
}
