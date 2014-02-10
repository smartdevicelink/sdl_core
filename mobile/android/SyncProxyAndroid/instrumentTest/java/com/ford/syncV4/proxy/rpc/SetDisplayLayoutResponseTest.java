package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.marshal.JsonRPCMarshaller;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

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

    public void testMessageShouldBeCreated() {
        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse();

        final DisplayCapabilities capabilities = new DisplayCapabilities();

        msg.setDisplayCapabilities(capabilities);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        assertThat(jsonObject.has(DISPLAY_CAPABILITIES), is(true));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        SetDisplayLayoutResponse msg = new SetDisplayLayoutResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertThat(msg, notNullValue());
        assertThat(msg.getDisplayCapabilities(), nullValue());
    }

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
}
