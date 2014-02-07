package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.rpc.enums.RequestType;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for SystemRequest class.
 *
 * Created by enikolsky on 2014-01-23.
 */
public class SystemRequestTest extends TestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final String REQUEST_TYPE = "requestType";

    public void testMessageShouldBeCreated() {
        SystemRequest msg = new SystemRequest();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        SystemRequest msg = new SystemRequest();

        final RequestType requestType = RequestType.AUTH_REQUEST;

        msg.setRequestType(requestType);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        assertThat(jsonObject.getString(REQUEST_TYPE),
                is(requestType.toString()));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        SystemRequest msg = new SystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(msg, notNullValue());
        assertThat(msg.getRequestType(), nullValue());
    }

    public void testGetRequestTypeShouldReturnSetValue() throws JSONException {
        SystemRequest msg = new SystemRequest();

        final RequestType requestType = RequestType.AUTH_ACK;
        msg.setRequestType(requestType);

        assertThat(msg.getRequestType(), is(requestType));
    }

    public void testSettingNullRequestTypeShouldRemoveValue()
            throws JSONException {
        SystemRequest msg = new SystemRequest();

        msg.setRequestType(RequestType.AUTH_CHALLENGE);
        msg.setRequestType(null);

        assertThat(msg.getRequestType(), nullValue());
    }

    public void testDeserializedRequestTypeShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final RequestType requestType = RequestType.AUTH_ACK;
        jsonObject.put(REQUEST_TYPE, requestType);

        SystemRequest msg = new SystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getRequestType(), is(requestType));
    }

    public void testDeserializedRequestTypeAsStringShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final RequestType requestType = RequestType.AUTH_ACK;
        jsonObject.put(REQUEST_TYPE, requestType.toString());

        SystemRequest msg = new SystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getRequestType(), is(requestType));
    }

    public void testDeserializedRequestTypeAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(REQUEST_TYPE, 1);

        SystemRequest msg = new SystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getRequestType(), nullValue());
    }
}
