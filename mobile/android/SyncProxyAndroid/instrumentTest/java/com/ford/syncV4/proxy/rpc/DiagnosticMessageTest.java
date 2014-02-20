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
 * Tests for DiagnosticMessage request.
 *
 * Created by enikolsky on 2014-02-17.
 */
public class DiagnosticMessageTest extends TestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final String TARGET_ID = "targetID";
    private static final String MESSAGE_LENGTH = "messageLength";
    private static final String MESSAGE_DATA = "messageData";

    public void testMessageShouldBeCreated() {
        DiagnosticMessage msg = new DiagnosticMessage();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        DiagnosticMessage msg = new DiagnosticMessage();

        final int targetID = 88;
        final int messageLength = 44;
        final Vector<Integer> messageData =
                new Vector<Integer>(Arrays.asList(22, 11));

        msg.setTargetID(targetID);
        msg.setMessageLength(messageLength);
        msg.setMessageData(messageData);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        assertThat(jsonObject.getInt(TARGET_ID), is(targetID));
        assertThat(jsonObject.getInt(MESSAGE_LENGTH), is(messageLength));
        final JSONArray messageDataJsonArray =
                jsonObject.getJSONArray(MESSAGE_DATA);
        assertThat(messageDataJsonArray.length(), is(messageData.size()));
        assertThat(messageDataJsonArray.getInt(0), is(messageData.get(0)));
        assertThat(messageDataJsonArray.getInt(1), is(messageData.get(1)));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        DiagnosticMessage msg = new DiagnosticMessage(
                TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(msg, notNullValue());
        assertThat(msg.getTargetID(), nullValue());
        assertThat(msg.getMessageLength(), nullValue());
        assertThat(msg.getMessageData(), nullValue());
    }

    /// targetID
    public void testGetTargetIDShouldReturnSetValue() throws JSONException {
        DiagnosticMessage msg = new DiagnosticMessage();

        final int targetID = 88;
        msg.setTargetID(targetID);

        assertThat(msg.getTargetID(), is(targetID));
    }

    public void testSettingNullTargetIDShouldRemoveValue()
            throws JSONException {
        DiagnosticMessage msg = new DiagnosticMessage();

        msg.setTargetID(42);
        msg.setTargetID(null);

        assertThat(msg.getTargetID(), nullValue());
    }

    public void testDeserializedTargetIDAsIntShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final int targetID = 88;
        jsonObject.put(TARGET_ID, targetID);

        DiagnosticMessage msg = new DiagnosticMessage(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getTargetID(), is(targetID));
    }

    public void testDeserializedTargetIDAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(TARGET_ID, "88");

        DiagnosticMessage msg = new DiagnosticMessage(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getTargetID(), nullValue());
    }

    /// messageLength
    public void testGetMessageLengthShouldReturnSetValue()
            throws JSONException {
        DiagnosticMessage msg = new DiagnosticMessage();

        final int messageLength = 44;
        msg.setMessageLength(messageLength);

        assertThat(msg.getMessageLength(), is(messageLength));
    }

    public void testSettingNullMessageLengthShouldRemoveValue()
            throws JSONException {
        DiagnosticMessage msg = new DiagnosticMessage();

        msg.setMessageLength(44);
        msg.setMessageLength(null);

        assertThat(msg.getMessageLength(), nullValue());
    }

    public void testDeserializedMessageLengthAsIntShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final int messageLength = 44;
        jsonObject.put(MESSAGE_LENGTH, messageLength);

        DiagnosticMessage msg = new DiagnosticMessage(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getMessageLength(), is(messageLength));
    }

    public void testDeserializedMessageLengthAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(MESSAGE_LENGTH, "88");

        DiagnosticMessage msg = new DiagnosticMessage(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getMessageLength(), nullValue());
    }

    /// messageData
    public void testGetMessageDataShouldReturnSetValue() throws JSONException {
        DiagnosticMessage msg = new DiagnosticMessage();

        final Vector<Integer> messageData =
                new Vector<Integer>(Arrays.asList(22, 11));
        msg.setMessageData(messageData);

        assertThat(msg.getMessageData(), is(messageData));
    }

    public void testSettingNullMessageDataShouldRemoveValue()
            throws JSONException {
        DiagnosticMessage msg = new DiagnosticMessage();

        msg.setMessageData(new Vector<Integer>());
        msg.setMessageData(null);

        assertThat(msg.getMessageData(), nullValue());
    }

    public void testDeserializedMessageDataAsVectorShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Vector<Integer> messageData =
                new Vector<Integer>(Arrays.asList(22, 11));
        jsonObject.put(MESSAGE_DATA, messageData);

        DiagnosticMessage msg = new DiagnosticMessage(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getMessageData(), is(messageData));
    }

    public void testDeserializedMessageDataAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(MESSAGE_DATA, 88);

        DiagnosticMessage msg = new DiagnosticMessage(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getMessageData(), nullValue());
    }

    public void testDeserializedMessageDataAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(MESSAGE_DATA, "88");

        DiagnosticMessage msg = new DiagnosticMessage(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getMessageData(), nullValue());
    }
}
