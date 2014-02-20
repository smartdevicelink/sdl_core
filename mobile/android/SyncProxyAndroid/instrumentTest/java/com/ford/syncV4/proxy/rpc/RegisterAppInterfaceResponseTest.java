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
    private static final String AUDIO_PASS_THRU_CAPABILITIES =
            "audioPassThruCapabilities";
    private static final String SUPPORTED_DIAG_MODES = "supportedDiagModes";

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
        final Vector<AudioPassThruCapabilities> audioPassThruCapabilities =
                new Vector<AudioPassThruCapabilities>(
                        Arrays.asList(new AudioPassThruCapabilities()));
        final Vector<Integer> supportedDiagModes =
                new Vector<Integer>(Arrays.asList(0x00, 0xFF));

        msg.setPrerecordedSpeech(speech);
        msg.setAudioPassThruCapabilities(audioPassThruCapabilities);
        msg.setSupportedDiagModes(supportedDiagModes);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        final JSONArray speechJsonArray =
                jsonObject.getJSONArray(PRERECORDED_SPEECH);
        assertThat(speechJsonArray.length(), is(speech.size()));
        assertThat(speechJsonArray.getString(0), is(speech.get(0).toString()));
        assertThat(speechJsonArray.getString(1), is(speech.get(1).toString()));
        assertThat(
                jsonObject.getJSONArray(AUDIO_PASS_THRU_CAPABILITIES).length(),
                is(audioPassThruCapabilities.size()));
        final JSONArray modesJsonArray =
                jsonObject.getJSONArray(SUPPORTED_DIAG_MODES);
        assertThat(modesJsonArray.length(), is(supportedDiagModes.size()));
        assertThat(modesJsonArray.getInt(0), is(supportedDiagModes.get(0)));
        assertThat(modesJsonArray.getInt(1), is(supportedDiagModes.get(1)));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertThat(msg, notNullValue());
        assertThat(msg.getPrerecordedSpeech(), nullValue());
        assertThat(msg.getAudioPassThruCapabilities(), nullValue());
        assertThat(msg.getSupportedDiagModes(), nullValue());
    }

    /// prerecordedSpeech
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

    /// audioPassThruCapabilities
    public void testAudioPassThruCapabilitiesGetterShouldReturnSetValue()
            throws JSONException {
        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse();

        final Vector<AudioPassThruCapabilities> audioPassThruCapabilities =
                new Vector<AudioPassThruCapabilities>(
                        Arrays.asList(new AudioPassThruCapabilities()));
        msg.setAudioPassThruCapabilities(audioPassThruCapabilities);

        assertThat(msg.getAudioPassThruCapabilities(),
                is(audioPassThruCapabilities));
    }

    public void testSettingNullAudioPassThruCapabilitiesShouldRemoveValue()
            throws JSONException {
        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse();

        msg.setAudioPassThruCapabilities(new Vector<AudioPassThruCapabilities>(
                Arrays.asList(new AudioPassThruCapabilities())));
        msg.setAudioPassThruCapabilities(null);

        assertThat(msg.getAudioPassThruCapabilities(), nullValue());
    }

    public void testDeserializedAudioPassThruCapabilitiesAsVectorShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Vector<AudioPassThruCapabilities> audioPassThruCapabilities =
                new Vector<AudioPassThruCapabilities>(
                        Arrays.asList(new AudioPassThruCapabilities()));
        jsonObject.put(AUDIO_PASS_THRU_CAPABILITIES, audioPassThruCapabilities);

        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getAudioPassThruCapabilities(),
                is(audioPassThruCapabilities));
    }

    public void testDeserializedAudioPassThruCapabilitiesAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(AUDIO_PASS_THRU_CAPABILITIES, "INITIAL_JINGLE");

        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getAudioPassThruCapabilities(), nullValue());
    }

    public void testDeserializedAudioPassThruCapabilitiesAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(AUDIO_PASS_THRU_CAPABILITIES, 5);

        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getAudioPassThruCapabilities(), nullValue());
    }

    /// supportedDiagModes
    public void testSupportedDiagModesGetterShouldReturnSetValue()
            throws JSONException {
        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse();

        final Vector<Integer> supportedDiagModes =
                new Vector<Integer>(Arrays.asList(0x00, 0xFF));
        msg.setSupportedDiagModes(supportedDiagModes);

        assertThat(msg.getSupportedDiagModes(), is(supportedDiagModes));
    }

    public void testSettingNullSupportedDiagModesShouldRemoveValue()
            throws JSONException {
        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse();

        msg.setSupportedDiagModes(
                new Vector<Integer>(Arrays.asList(0x00, 0xFF)));
        msg.setSupportedDiagModes(null);

        assertThat(msg.getSupportedDiagModes(), nullValue());
    }

    public void testDeserializedSupportedDiagModesAsVectorShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Vector<Integer> supportedDiagModes =
                new Vector<Integer>(Arrays.asList(0x00, 0xFF));
        jsonObject.put(SUPPORTED_DIAG_MODES, supportedDiagModes);

        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSupportedDiagModes(), is(supportedDiagModes));
    }

    public void testDeserializedSupportedDiagModesAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(SUPPORTED_DIAG_MODES, "INITIAL_JINGLE");

        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSupportedDiagModes(), nullValue());
    }

    public void testDeserializedSupportedDiagModesAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(SUPPORTED_DIAG_MODES, 5);

        RegisterAppInterfaceResponse msg = new RegisterAppInterfaceResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSupportedDiagModes(), nullValue());
    }
}
