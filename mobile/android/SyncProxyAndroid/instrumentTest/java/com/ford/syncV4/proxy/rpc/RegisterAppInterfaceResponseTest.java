package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.rpc.enums.PrerecordedSpeech;

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
 * Tests for RegisterAppInterfaceResponse response (new params only).
 *
 * Created by enikolsky on 2014-02-11.
 */
public class RegisterAppInterfaceResponseTest extends TestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final String PRERECORDED_SPEECH = "prerecordedSpeech";

    public void testMessageShouldBeCreated() {
        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse();

        final Vector<PrerecordedSpeech> speech = new Vector<PrerecordedSpeech>(
                Arrays.asList(PrerecordedSpeech.HELP_JINGLE,
                        PrerecordedSpeech.INITIAL_JINGLE));

        msg.setPrerecordedSpeech(speech);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        final JSONArray speechJsonArray =
                jsonObject.getJSONArray(PRERECORDED_SPEECH);
        assertThat(speechJsonArray.length(), is(speech.size()));
        assertThat(speechJsonArray.getString(0), is(speech.get(0).toString()));
        assertThat(speechJsonArray.getString(1), is(speech.get(1).toString()));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertThat(msg, notNullValue());
        assertThat(msg.getPrerecordedSpeech(), nullValue());
    }

    public void testPrerecordedSpeechGetterShouldReturnSetValue()
            throws JSONException {
        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse();

        final Vector<PrerecordedSpeech> speech = new Vector<PrerecordedSpeech>(
                Arrays.asList(PrerecordedSpeech.HELP_JINGLE,
                        PrerecordedSpeech.INITIAL_JINGLE));
        msg.setPrerecordedSpeech(speech);

        assertThat(msg.getPrerecordedSpeech(), is(speech));
    }

    public void testSettingNullPrerecordedSpeechShouldRemoveValue()
            throws JSONException {
        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse();

        msg.setPrerecordedSpeech(new Vector<PrerecordedSpeech>(
                Arrays.asList(PrerecordedSpeech.LISTEN_JINGLE)));
        msg.setPrerecordedSpeech(null);

        assertThat(msg.getPrerecordedSpeech(), nullValue());
    }

    public void testDeserializedPrerecordedSpeechAsVectorShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Vector<PrerecordedSpeech> speech = new Vector<PrerecordedSpeech>(
                Arrays.asList(PrerecordedSpeech.HELP_JINGLE,
                        PrerecordedSpeech.INITIAL_JINGLE));
        jsonObject.put(PRERECORDED_SPEECH, speech);

        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getPrerecordedSpeech(), is(speech));
    }

    public void testDeserializedPrerecordedSpeechAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(PRERECORDED_SPEECH, "INITIAL_JINGLE");

        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getPrerecordedSpeech(), nullValue());
    }

    public void testDeserializedPrerecordedSpeechAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(PRERECORDED_SPEECH, 5);

        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getPrerecordedSpeech(), nullValue());
    }
}
