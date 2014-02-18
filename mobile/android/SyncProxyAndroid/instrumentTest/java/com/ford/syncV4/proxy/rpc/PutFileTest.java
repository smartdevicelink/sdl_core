package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.RPCRequestFactory;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for PutFile class (only new parameters at the moment).
 *
 * Created by enikolsky on 2014-01-20.
 */
public class PutFileTest extends TestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final String SYSTEM_FILE = "systemFile";
    private static final String OFFSET = "offset";
    private static final String LENGTH = "length";

    public void testMessageShouldBeCreated() {
        PutFile msg = RPCRequestFactory.buildPutFile();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        PutFile msg = RPCRequestFactory.buildPutFile();

        final boolean systemFile = true;
        final Integer offset = 531;
        final Integer length = 1000;

        msg.setSystemFile(systemFile);
        msg.setOffset(offset);
        msg.setLength(length);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        assertThat(jsonObject.getBoolean(SYSTEM_FILE), is(systemFile));
        assertThat(jsonObject.getInt(OFFSET), is(offset));
        assertThat(jsonObject.getInt(LENGTH), is(length));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        PutFile msg = new PutFile(JsonRPCMarshaller.deserializeJSONObject(
                TestCommon.paramsToRequestObject(jsonObject)));

        assertThat(msg, notNullValue());
        assertThat(msg.getSystemFile(), nullValue());
        assertThat(msg.getOffset(), nullValue());
        assertThat(msg.getLength(), nullValue());
    }

    public void testSystemFileGetterShouldReturnSetValue()
            throws JSONException {
        PutFile msg = RPCRequestFactory.buildPutFile();

        final boolean systemFile = true;
        msg.setSystemFile(systemFile);

        assertThat(msg.getSystemFile(), is(systemFile));
    }

    public void testSettingNullSystemFileShouldRemoveValue()
            throws JSONException {
        PutFile msg = RPCRequestFactory.buildPutFile();

        msg.setSystemFile(true);
        msg.setSystemFile(null);

        assertThat(msg.getSystemFile(), nullValue());
    }

    public void testDeserializedSystemFileShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final boolean systemFile = true;
        jsonObject.put(SYSTEM_FILE, systemFile);

        PutFile msg = new PutFile(JsonRPCMarshaller.deserializeJSONObject(
                TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSystemFile(), is(systemFile));
    }

    public void testDeserializedSystemFileAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(SYSTEM_FILE, 42);

        PutFile msg = new PutFile(JsonRPCMarshaller.deserializeJSONObject(
                TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSystemFile(), nullValue());
    }

    public void testOffsetGetterShouldReturnSetValue() throws JSONException {
        PutFile msg = RPCRequestFactory.buildPutFile();

        final Integer offset = 333;
        msg.setOffset(offset);

        assertThat(msg.getOffset(), is(offset));
    }

    public void testSettingNullOffsetShouldRemoveValue() throws JSONException {
        PutFile msg = RPCRequestFactory.buildPutFile();

        msg.setOffset(333);
        msg.setOffset(null);

        assertThat(msg.getOffset(), nullValue());
    }

    public void testDeserializedOffsetShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Integer offset = 333;
        jsonObject.put(OFFSET, offset);

        PutFile msg = new PutFile(JsonRPCMarshaller.deserializeJSONObject(
                TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getOffset(), is(offset));
    }

    public void testDeserializedOffsetAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(OFFSET, "333");

        PutFile msg = new PutFile(JsonRPCMarshaller.deserializeJSONObject(
                TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getOffset(), nullValue());
    }

    public void testLengthGetterShouldReturnSetValue() throws JSONException {
        PutFile msg = RPCRequestFactory.buildPutFile();

        final Integer length = 333;
        msg.setLength(length);

        assertThat(msg.getLength(), is(length));
    }

    public void testSettingNullLengthShouldRemoveValue() throws JSONException {
        PutFile msg = RPCRequestFactory.buildPutFile();

        msg.setLength(333);
        msg.setLength(null);

        assertThat(msg.getLength(), nullValue());
    }

    public void testDeserializedLengthShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Integer length = 333;
        jsonObject.put(LENGTH, length);

        PutFile msg = new PutFile(JsonRPCMarshaller.deserializeJSONObject(
                TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getLength(), is(length));
    }

    public void testDeserializedLengthAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(LENGTH, "333");

        PutFile msg = new PutFile(JsonRPCMarshaller.deserializeJSONObject(
                TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getLength(), nullValue());
    }
}
