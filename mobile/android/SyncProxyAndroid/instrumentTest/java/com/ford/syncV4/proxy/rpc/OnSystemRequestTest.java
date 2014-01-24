package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;

import junit.framework.TestCase;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.Vector;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for OnOnSystemRequest class.
 *
 * Created by enikolsky on 2014-01-24.
 */
public class OnSystemRequestTest extends TestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final String REQUEST_TYPE = "requestType";
    private static final String FILE_TYPE = "fileType";
    private static final String OFFSET = "offset";
    private static final String LENGTH = "length";
    private static final String URL = "url";

    public void testMessageShouldBeCreated() {
        OnSystemRequest msg = new OnSystemRequest();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        OnSystemRequest msg = new OnSystemRequest();

        final RequestType requestType = RequestType.AUTH_REQUEST;
        final Vector<String> urls = new Vector<String>();
        urls.add("https://example.com/");
        final FileType fileType = FileType.GRAPHIC_PNG;
        final int offset = 0;
        final int length = 1500;

        msg.setRequestType(requestType);
        msg.setUrl(urls);
        msg.setFileType(fileType);
        msg.setOffset(offset);
        msg.setLength(length);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        assertThat(jsonObject.getString(REQUEST_TYPE),
                is(requestType.toString()));
        final JSONArray urlJSONArray = jsonObject.getJSONArray(URL);
        assertThat(urlJSONArray.length(), is(urls.size()));
        assertThat(urlJSONArray.getString(0), is(urls.get(0)));
        assertThat(jsonObject.getString(FILE_TYPE), is(fileType.toString()));
        assertThat(jsonObject.getInt(OFFSET), is(offset));
        assertThat(jsonObject.getInt(LENGTH), is(length));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        OnSystemRequest msg = new OnSystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(msg, notNullValue());
        assertThat(msg.getRequestType(), nullValue());
        assertThat(msg.getUrl(), nullValue());
        assertThat(msg.getFileType(), nullValue());
        assertThat(msg.getOffset(), nullValue());
        assertThat(msg.getLength(), nullValue());
    }

    public void testGetRequestTypeShouldReturnSetValue() throws JSONException {
        OnSystemRequest msg = new OnSystemRequest();

        final RequestType requestType = RequestType.AUTH_ACK;
        msg.setRequestType(requestType);

        assertThat(msg.getRequestType(), is(requestType));
    }

    public void testSettingNullRequestTypeShouldRemoveValue()
            throws JSONException {
        OnSystemRequest msg = new OnSystemRequest();

        msg.setRequestType(RequestType.AUTH_CHALLENGE);
        msg.setRequestType(null);

        assertThat(msg.getRequestType(), nullValue());
    }

    public void testDeserializedRequestTypeShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final RequestType requestType = RequestType.AUTH_ACK;
        jsonObject.put(REQUEST_TYPE, requestType);

        OnSystemRequest msg = new OnSystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getRequestType(), is(requestType));
    }

    public void testDeserializedRequestTypeAsStringShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final RequestType requestType = RequestType.AUTH_ACK;
        jsonObject.put(REQUEST_TYPE, requestType.toString());

        OnSystemRequest msg = new OnSystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getRequestType(), is(requestType));
    }

    public void testDeserializedRequestTypeAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(REQUEST_TYPE, 1);

        OnSystemRequest msg = new OnSystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getRequestType(), nullValue());
    }

    public void testGetUrlShouldReturnSetValue() throws JSONException {
        OnSystemRequest msg = new OnSystemRequest();

        final Vector<String> urls = new Vector<String>();
        urls.add("https://d.android.com/");
        msg.setUrl(urls);

        assertThat(msg.getUrl(), is(urls));
    }

    public void testSettingNullUrlShouldRemoveValue() throws JSONException {
        OnSystemRequest msg = new OnSystemRequest();

        final Vector<String> urls = new Vector<String>();
        urls.add("https://d.android.com/");
        msg.setUrl(urls);
        msg.setUrl(null);

        assertThat(msg.getUrl(), nullValue());
    }

    public void testDeserializedUrlShouldContainValue() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Vector<String> urls = new Vector<String>();
        urls.add("https://d.android.com/");
        jsonObject.put(URL, urls);

        OnSystemRequest msg = new OnSystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getUrl(), is(urls));
    }

    public void testDeserializedUrlAsStringShouldBeNull() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(URL, "https://d.android.com/");

        OnSystemRequest msg = new OnSystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getUrl(), nullValue());
    }

    public void testDeserializedUrlAsIntShouldBeNull() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(URL, 1);

        OnSystemRequest msg = new OnSystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getUrl(), nullValue());
    }

    public void testGetFileTypeShouldReturnSetValue() throws JSONException {
        OnSystemRequest msg = new OnSystemRequest();

        final FileType fileType = FileType.GRAPHIC_PNG;
        msg.setFileType(fileType);

        assertThat(msg.getFileType(), is(fileType));
    }

    public void testSettingNullFileTypeShouldRemoveValue()
            throws JSONException {
        OnSystemRequest msg = new OnSystemRequest();

        msg.setFileType(FileType.AUDIO_AAC);
        msg.setFileType(null);

        assertThat(msg.getFileType(), nullValue());
    }

    public void testDeserializedFileTypeShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final FileType fileType = FileType.AUDIO_MP3;
        jsonObject.put(FILE_TYPE, fileType);

        OnSystemRequest msg = new OnSystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getFileType(), is(fileType));
    }

    public void testDeserializedFileTypeAsStringShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final FileType fileType = FileType.AUDIO_WAVE;
        jsonObject.put(FILE_TYPE, fileType.toString());

        OnSystemRequest msg = new OnSystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getFileType(), is(fileType));
    }

    public void testDeserializedFileTypeAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(FILE_TYPE, 1);

        OnSystemRequest msg = new OnSystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getFileType(), nullValue());
    }

    public void testGetOffsetShouldReturnSetValue() throws JSONException {
        OnSystemRequest msg = new OnSystemRequest();

        final int offset = 88;
        msg.setOffset(offset);

        assertThat(msg.getOffset(), is(offset));
    }

    public void testSettingNullOffsetShouldRemoveValue() throws JSONException {
        OnSystemRequest msg = new OnSystemRequest();

        msg.setOffset(12);
        msg.setOffset(null);

        assertThat(msg.getOffset(), nullValue());
    }

    public void testDeserializedOffsetShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final int Offset = 88;
        jsonObject.put(OFFSET, Offset);

        OnSystemRequest msg = new OnSystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getOffset(), is(Offset));
    }

    public void testDeserializedOffsetAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(OFFSET, "88");

        OnSystemRequest msg = new OnSystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getOffset(), nullValue());
    }

    public void testGetLengthShouldReturnSetValue() throws JSONException {
        OnSystemRequest msg = new OnSystemRequest();

        final int length = 88;
        msg.setLength(length);

        assertThat(msg.getLength(), is(length));
    }

    public void testSettingNullLengthShouldRemoveValue() throws JSONException {
        OnSystemRequest msg = new OnSystemRequest();

        msg.setLength(12);
        msg.setLength(null);

        assertThat(msg.getLength(), nullValue());
    }

    public void testDeserializedLengthShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final int length = 88;
        jsonObject.put(LENGTH, length);

        OnSystemRequest msg = new OnSystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getLength(), is(length));
    }

    public void testDeserializedLengthAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(LENGTH, "88");

        OnSystemRequest msg = new OnSystemRequest(
                TestCommon.deserializeJSONRequestObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getLength(), nullValue());
    }
}
