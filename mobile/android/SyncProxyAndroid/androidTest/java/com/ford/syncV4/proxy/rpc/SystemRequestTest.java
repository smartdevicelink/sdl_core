package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
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

    private static final byte PROTOCOL_VERSION = ProtocolConstants.PROTOCOL_VERSION_TWO;
    private static final String NAME_OF_THE_FILE = "ThisIsA NameOfThe file";

    public void testMessageShouldBeCreated() {
        SystemRequest msg = new SystemRequest();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields() throws JSONException {
        SystemRequest msg = new SystemRequest();

        final RequestType requestType = RequestType.AUTH_REQUEST;

        msg.setRequestType(requestType);
        msg.setFileName(NAME_OF_THE_FILE);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        assertThat(jsonObject.getString(Names.requestType), is(requestType.toString()));
        assertThat(jsonObject.getString(Names.fileName), is(NAME_OF_THE_FILE));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        SystemRequest msg = new SystemRequest(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(msg, notNullValue());
        assertThat(msg.getRequestType(), nullValue());
        assertThat(msg.getFileName(), nullValue());
    }

    public void testGetRequestTypeShouldReturnSetValue() throws JSONException {
        SystemRequest msg = new SystemRequest();

        final RequestType requestType = RequestType.AUTH_ACK;
        msg.setRequestType(requestType);

        assertThat(msg.getRequestType(), is(requestType));
    }

    public void testSettingNullRequestTypeShouldRemoveValue() throws JSONException {
        SystemRequest msg = new SystemRequest();

        msg.setRequestType(RequestType.AUTH_CHALLENGE);
        msg.setRequestType(null);

        assertThat(msg.getRequestType(), nullValue());
    }

    public void testGetFileNameShouldReturnSetValue() throws JSONException {
        SystemRequest msg = new SystemRequest();

        msg.setFileName(NAME_OF_THE_FILE);

        assertThat(msg.getFileName(), is(NAME_OF_THE_FILE));
    }

    public void testSettingNullFileNameShouldRemoveValue() throws JSONException {
        SystemRequest msg = new SystemRequest();

        msg.setFileName(NAME_OF_THE_FILE);
        msg.setFileName(null);

        assertThat(msg.getFileName(), nullValue());
    }

    public void testDeserializedRequestTypeShouldContainValue() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final RequestType requestType = RequestType.AUTH_ACK;
        jsonObject.put(Names.requestType, requestType);
        jsonObject.put(Names.fileName, NAME_OF_THE_FILE);

        SystemRequest msg = new SystemRequest(TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getRequestType(), is(requestType));
        assertThat(msg.getFileName(), is(NAME_OF_THE_FILE));
    }

    public void testDeserializedRequestTypeAsStringShouldContainValue() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final RequestType requestType = RequestType.AUTH_ACK;
        jsonObject.put(Names.requestType, requestType.toString());
        jsonObject.put(Names.fileName, NAME_OF_THE_FILE);

        SystemRequest msg = new SystemRequest(TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getRequestType(), is(requestType));
        assertThat(msg.getFileName(), is(NAME_OF_THE_FILE));
    }

    public void testDeserializedRequestTypeAsIntShouldBeNull() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.requestType, 1);

        SystemRequest msg = new SystemRequest(TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getRequestType(), nullValue());
    }

    public void testDeserializedFileNameAsIntShouldBeNull() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.fileName, 1);

        SystemRequest msg = new SystemRequest(TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getFileName(), nullValue());
    }
}