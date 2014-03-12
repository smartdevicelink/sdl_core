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
 * Tests for ReadDIDResponse response.
 *
 * Created by enikolsky on 2014-02-13.
 */
public class ReadDIDResponseTest extends TestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final String DID_RESULT = "didResult";

    public void testMessageShouldBeCreated() {
        ReadDIDResponse msg = new ReadDIDResponse();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        ReadDIDResponse msg = new ReadDIDResponse();

        final Vector<DIDResult> results =
                new Vector<DIDResult>(Arrays.asList(new DIDResult()));

        msg.setDidResult(results);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        assertThat(jsonObject.has(DID_RESULT), is(true));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        ReadDIDResponse msg = new ReadDIDResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertThat(msg, notNullValue());
        assertThat(msg.getDidResult(), nullValue());
    }

    // didResult
    public void testDidResultGetterShouldReturnSetValue() throws JSONException {
        ReadDIDResponse msg = new ReadDIDResponse();

        final Vector<DIDResult> results =
                new Vector<DIDResult>(Arrays.asList(new DIDResult()));
        msg.setDidResult(results);

        assertThat(msg.getDidResult(), is(results));
    }

    public void testSettingNullDidResultShouldRemoveValue()
            throws JSONException {
        ReadDIDResponse msg = new ReadDIDResponse();

        msg.setDidResult(new Vector<DIDResult>(Arrays.asList(new DIDResult())));
        msg.setDidResult(null);

        assertThat(msg.getDidResult(), nullValue());
    }

    public void testDeserializedDidResultAsVectorShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Vector<DIDResult> results =
                new Vector<DIDResult>(Arrays.asList(new DIDResult()));
        jsonObject.put(DID_RESULT, results);

        ReadDIDResponse msg = new ReadDIDResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getDidResult(), is(results));
    }

    public void testDeserializedDidResultAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(DID_RESULT, "4.0f");

        ReadDIDResponse msg = new ReadDIDResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getDidResult(), nullValue());
    }

    public void testDeserializedDidResultAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(DID_RESULT, 12);

        ReadDIDResponse msg = new ReadDIDResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getDidResult(), nullValue());
    }
}
