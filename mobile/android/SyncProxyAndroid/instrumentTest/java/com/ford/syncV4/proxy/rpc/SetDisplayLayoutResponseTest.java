package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.marshal.JsonRPCMarshaller;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Arrays;
import java.util.Vector;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for SetDisplayLayoutResponse response.
 *
 * Created by enikolsky on 2014-02-10.
 */
public class SetDisplayLayoutResponseTest extends TestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final String DISPLAY_CAPABILITIES = "displayCapabilities";
    private static final String BUTTON_CAPABILITIES = "buttonCapabilities";
    private static final String SOFT_BUTTON_CAPABILITIES =
            "softButtonCapabilities";
    private static final String PRESET_BANK_CAPABILITIES =
            "presetBankCapabilities";

    public void testMessageShouldBeCreated() {
        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse();

        final DisplayCapabilities capabilities = new DisplayCapabilities();
        final Vector<ButtonCapabilities> buttonCapabilities =
                new Vector<ButtonCapabilities>(
                        Arrays.asList(new ButtonCapabilities()));
        final Vector<SoftButtonCapabilities> softButtonCapabilities =
                new Vector<SoftButtonCapabilities>(
                        Arrays.asList(new SoftButtonCapabilities()));
        final PresetBankCapabilities presetBankCapabilities =
                new PresetBankCapabilities();

        msg.setDisplayCapabilities(capabilities);
        msg.setButtonCapabilities(buttonCapabilities);
        msg.setSoftButtonCapabilities(softButtonCapabilities);
        msg.setPresetBankCapabilities(presetBankCapabilities);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        assertThat(jsonObject.has(DISPLAY_CAPABILITIES), is(true));
        assertThat(jsonObject.getJSONArray(BUTTON_CAPABILITIES).length(),
                is(buttonCapabilities.size()));
        assertThat(jsonObject.getJSONArray(SOFT_BUTTON_CAPABILITIES).length(),
                is(softButtonCapabilities.size()));
        assertThat(jsonObject.has(PRESET_BANK_CAPABILITIES), is(true));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertThat(msg, notNullValue());
        assertThat(msg.getDisplayCapabilities(), nullValue());
        assertThat(msg.getButtonCapabilities(), nullValue());
        assertThat(msg.getSoftButtonCapabilities(), nullValue());
        assertThat(msg.getPresetBankCapabilities(), nullValue());
    }

    // displayCapabilities
    public void testDisplayCapabilitiesGetterShouldReturnSetValue()
            throws JSONException {
        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse();

        final DisplayCapabilities capabilities = new DisplayCapabilities();
        msg.setDisplayCapabilities(capabilities);

        assertThat(msg.getDisplayCapabilities(), is(capabilities));
    }

    public void testSettingNullDisplayCapabilitiesShouldRemoveValue()
            throws JSONException {
        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse();

        msg.setDisplayCapabilities(new DisplayCapabilities());
        msg.setDisplayCapabilities(null);

        assertThat(msg.getDisplayCapabilities(), nullValue());
    }

    public void testDeserializedDisplayCapabilitiesAsStructShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final DisplayCapabilities capabilities = new DisplayCapabilities();
        jsonObject.put(DISPLAY_CAPABILITIES, capabilities);

        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getDisplayCapabilities(), is(capabilities));
    }

    public void testDeserializedDisplayCapabilitiesAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(DISPLAY_CAPABILITIES, "4.0f");

        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getDisplayCapabilities(), nullValue());
    }

    public void testDeserializedDisplayCapabilitiesAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(DISPLAY_CAPABILITIES, 5);

        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getDisplayCapabilities(), nullValue());
    }

    // buttonCapabilities
    public void testButtonCapabilitiesGetterShouldReturnSetValue()
            throws JSONException {
        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse();

        final Vector<ButtonCapabilities> buttonCapabilities =
                new Vector<ButtonCapabilities>(
                        Arrays.asList(new ButtonCapabilities()));
        msg.setButtonCapabilities(buttonCapabilities);

        assertThat(msg.getButtonCapabilities(), is(buttonCapabilities));
    }

    public void testSettingNullButtonCapabilitiesShouldRemoveValue()
            throws JSONException {
        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse();

        msg.setButtonCapabilities(new Vector<ButtonCapabilities>());
        msg.setButtonCapabilities(null);

        assertThat(msg.getButtonCapabilities(), nullValue());
    }

    public void testDeserializedButtonCapabilitiesAsVectorShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Vector<ButtonCapabilities> buttonCapabilities =
                new Vector<ButtonCapabilities>(
                        Arrays.asList(new ButtonCapabilities()));
        jsonObject.put(BUTTON_CAPABILITIES, buttonCapabilities);

        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getButtonCapabilities(), is(buttonCapabilities));
    }

    public void testDeserializedButtonCapabilitiesAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(BUTTON_CAPABILITIES, "4.0f");

        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getButtonCapabilities(), nullValue());
    }

    public void testDeserializedButtonCapabilitiesAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(BUTTON_CAPABILITIES, 5);

        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getButtonCapabilities(), nullValue());
    }

    // softButtonCapabilities
    public void testSoftButtonCapabilitiesGetterShouldReturnSetValue()
            throws JSONException {
        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse();

        final Vector<SoftButtonCapabilities> softButtonCapabilities =
                new Vector<SoftButtonCapabilities>(
                        Arrays.asList(new SoftButtonCapabilities()));
        msg.setSoftButtonCapabilities(softButtonCapabilities);

        assertThat(msg.getSoftButtonCapabilities(), is(softButtonCapabilities));
    }

    public void testSettingNullSoftButtonCapabilitiesShouldRemoveValue()
            throws JSONException {
        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse();

        msg.setSoftButtonCapabilities(new Vector<SoftButtonCapabilities>());
        msg.setSoftButtonCapabilities(null);

        assertThat(msg.getSoftButtonCapabilities(), nullValue());
    }

    public void testDeserializedSoftButtonCapabilitiesAsVectorShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Vector<SoftButtonCapabilities> softButtonCapabilities =
                new Vector<SoftButtonCapabilities>(
                        Arrays.asList(new SoftButtonCapabilities()));
        jsonObject.put(SOFT_BUTTON_CAPABILITIES, softButtonCapabilities);

        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSoftButtonCapabilities(), is(softButtonCapabilities));
    }

    public void testDeserializedSoftButtonCapabilitiesAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(SOFT_BUTTON_CAPABILITIES, "4.0f");

        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSoftButtonCapabilities(), nullValue());
    }

    public void testDeserializedSoftButtonCapabilitiesAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(SOFT_BUTTON_CAPABILITIES, 5);

        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSoftButtonCapabilities(), nullValue());
    }

    // presetBankCapabilities
    public void testPresetBankCapabilitiesGetterShouldReturnSetValue()
            throws JSONException {
        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse();

        final PresetBankCapabilities capabilities =
                new PresetBankCapabilities();
        msg.setPresetBankCapabilities(capabilities);

        assertThat(msg.getPresetBankCapabilities(), is(capabilities));
    }

    public void testSettingNullPresetBankCapabilitiesShouldRemoveValue()
            throws JSONException {
        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse();

        msg.setPresetBankCapabilities(new PresetBankCapabilities());
        msg.setPresetBankCapabilities(null);

        assertThat(msg.getPresetBankCapabilities(), nullValue());
    }

    public void testDeserializedPresetBankCapabilitiesAsStructShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final PresetBankCapabilities capabilities =
                new PresetBankCapabilities();
        jsonObject.put(PRESET_BANK_CAPABILITIES, capabilities);

        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getPresetBankCapabilities(), is(capabilities));
    }

    public void testDeserializedPresetBankCapabilitiesAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(PRESET_BANK_CAPABILITIES, "4.0f");

        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getPresetBankCapabilities(), nullValue());
    }

    public void testDeserializedPresetBankCapabilitiesAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(PRESET_BANK_CAPABILITIES, 5);

        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getPresetBankCapabilities(), nullValue());
    }
}
