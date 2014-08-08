package com.ford.syncV4.net.parser;

import com.ford.syncV4.net.HttpRequestParameters;
import com.ford.syncV4.util.logger.Logger;

import junit.framework.TestCase;

import org.hamcrest.CoreMatchers;
import org.hamcrest.MatcherAssert;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.nio.charset.Charset;
import java.util.Iterator;
import java.util.Set;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 08.08.14
 * Time: 10:36
 */
public class RequestJSONDataParserTest extends TestCase {

    /**
     * Number of the headers in the valid JSON Object
     */
    private static final int HEADERS_COUNT = 10;

    private static final String BODY_KEY = "data";
    private static final String BODY_VALUE = "HwcZAAAnMERRN1kxNjg2AAAAAQAAAAAAMDEyMzQ1Njc4OTAx" +
            "MjM0NaUXfMEJcxkCqizvE8/NX6yeBfflBJBWJ+CsODgc6Panl/24vXk1xUDbtj2bAdwTcADrP08n9c7qG1C" +
            "F9tiOe0UrdF3qW/7ChakvcCvn0IP55jNGhZLvHyDh1Clo35NJU6DGU2aetcOKm9lMAF";

    private static final String BODY_DATA = "{\\\"" + BODY_KEY + "\\\":[\\\"" + BODY_VALUE + "\\\"]}";

    private static final String REQUEST_METHOD = "POST";

    public void testValidRawDataToJSONObjectNotNull() {
        byte[] rawData = getValidJSONDataAsString().getBytes(Charset.forName("UTF-8"));

        assertNotNull(RequestJSONDataParser.getJSONFromRawData(rawData));
    }

    public void testValidRawDataToJSONObjectCorrectInstanceOf() {
        byte[] rawData = getValidJSONDataAsString().getBytes(Charset.forName("UTF-8"));

        MatcherAssert.assertThat(RequestJSONDataParser.getJSONFromRawData(rawData),
                CoreMatchers.instanceOf(JSONObject.class));
    }

    public void testInvalidRawDataToJSONObjectNotNull() {
        byte[] rawData = null;

        assertNotNull(RequestJSONDataParser.getJSONFromRawData(rawData));
        MatcherAssert.assertThat(RequestJSONDataParser.getJSONFromRawData(rawData),
                CoreMatchers.instanceOf(JSONObject.class));

        rawData = new byte[0];

        assertNotNull(RequestJSONDataParser.getJSONFromRawData(rawData));
        MatcherAssert.assertThat(RequestJSONDataParser.getJSONFromRawData(rawData),
                CoreMatchers.instanceOf(JSONObject.class));

    }

    public void testInvalidRawDataToJSONObjectIsEmpty() {
        byte[] rawData = null;

        assertNotNull(RequestJSONDataParser.getJSONFromRawData(rawData));
        MatcherAssert.assertThat(RequestJSONDataParser.getJSONFromRawData(rawData),
                CoreMatchers.instanceOf(JSONObject.class));

        rawData = new byte[0];

        assertNotNull(RequestJSONDataParser.getJSONFromRawData(rawData));
        MatcherAssert.assertThat(RequestJSONDataParser.getJSONFromRawData(rawData),
                CoreMatchers.instanceOf(JSONObject.class));

        JSONObject jsonFromRawData = RequestJSONDataParser.getJSONFromRawData(rawData);
        Iterator iterator = jsonFromRawData.keys();
        assertFalse(iterator.hasNext());
    }

    public void testGetHTTPRequestParametersInvalidInputReturnNotNull() throws JSONException {
        byte[] rawData = null;

        RequestAbstractDataParser parser = new RequestJSONDataParser();
        HttpRequestParameters parameters = parser.getHTTPRequestParameters(rawData);

        assertNotNull(parameters);
    }

    public void testGetHTTPRequestParametersContainsHeaders() throws JSONException {
        byte[] rawData = getValidJSONDataAsString().getBytes(Charset.forName("UTF-8"));

        RequestAbstractDataParser parser = new RequestJSONDataParser();
        HttpRequestParameters parameters = parser.getHTTPRequestParameters(rawData);

        Set<String> headers = parameters.getRequestHeadersKeysSet();

        assertNotNull(headers);
        assertEquals(HEADERS_COUNT, headers.size());
        assertTrue(headers.contains(RequestAbstractDataParser.KEY_CONTENT_TYPE));
        assertTrue(headers.contains(RequestAbstractDataParser.KEY_CHARSET));
        assertTrue(headers.contains(RequestAbstractDataParser.KEY_CONNECT_TIMEOUT));
        assertTrue(headers.contains(RequestAbstractDataParser.KEY_DO_INPUT));
        assertTrue(headers.contains(RequestAbstractDataParser.KEY_DO_OUTPUT));
        assertTrue(headers.contains(RequestAbstractDataParser.KEY_USE_CACHES));
        assertTrue(headers.contains(RequestAbstractDataParser.KEY_REQUEST_METHOD));
        assertTrue(headers.contains(RequestAbstractDataParser.KEY_READ_TIMEOUT));
        assertTrue(headers.contains(RequestAbstractDataParser.KEY_INSTANCE_FOLLOW_REDIRECTS));
        assertTrue(headers.contains(RequestAbstractDataParser.KEY_CONTENT_LENGTH));
    }

    public void testGetHTTPRequestParametersContainsBody() throws JSONException {
        byte[] rawData = getValidJSONDataAsString().getBytes(Charset.forName("UTF-8"));

        RequestAbstractDataParser parser = new RequestJSONDataParser();
        HttpRequestParameters parameters = parser.getHTTPRequestParameters(rawData);

        JSONObject body = parameters.getRequestBody();

        assertNotNull(body);
    }

    public void testGetHTTPRequestParametersContainsBodyWithCorrectData() throws JSONException {
        byte[] rawData = getValidJSONDataAsString().getBytes(Charset.forName("UTF-8"));

        RequestAbstractDataParser parser = new RequestJSONDataParser();
        HttpRequestParameters parameters = parser.getHTTPRequestParameters(rawData);

        JSONObject body = parameters.getRequestBody();

        assertNotNull(body);
        assertTrue(body.has(BODY_KEY));
        assertEquals("[\"" + BODY_VALUE + "\"]", body.getString(BODY_KEY).replace("\\", ""));
    }

    public void testGetHTTPRequestParametersContainsRequestMethod() throws JSONException {
        byte[] rawData = getValidJSONDataAsString().getBytes(Charset.forName("UTF-8"));

        RequestAbstractDataParser parser = new RequestJSONDataParser();
        HttpRequestParameters parameters = parser.getHTTPRequestParameters(rawData);

        assertEquals(REQUEST_METHOD, parameters.getRequestMethod().toString());
    }

    private static String getValidJSONDataAsString() {
        return "{\"HTTPRequest\":{\"headers\":{\"ContentType\":\"application/json\"," +
                "\"ConnectTimeout\":60,\"DoOutput\":true,\"DoInput\":true,\"UseCaches\":false," +
                "\"RequestMethod\":\"" + REQUEST_METHOD + "\",\"ReadTimeout\":60,\"InstanceFollowRedirects\":false," +
                "\"charset\":\"utf-8\",\"Content-Length\":13465},\"body\":\"" + BODY_DATA + "\"}}";
    }

}