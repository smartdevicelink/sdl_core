package com.ford.syncV4.net;

import android.net.Uri;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 08.08.14
 * Time: 16:37
 */
public class HttpRequestParametersTest extends TestCase {

    private HttpRequestParameters mParameters;

    @Override
    public void setUp() throws Exception {
        super.setUp();

        mParameters = new HttpRequestParameters();
    }

    public void testCreateInstanceShouldNotBeNull() {
        assertNotNull(mParameters);
    }

    public void testRequestMethodDefault() {
        assertEquals(HttpRequestParameters.REQUEST_METHOD.POST, mParameters.getRequestMethod());
    }

    public void testRequestBodyDefaultNotNull() {
        assertNotNull(mParameters.getRequestBody());
    }

    public void testRequestBody() throws JSONException {
        String body = "{'body':'qwertyuiop'}";
        JSONObject jsonObject = new JSONObject(body);

        mParameters.setRequestBody(jsonObject);

        assertEquals(jsonObject, mParameters.getRequestBody());
    }

    public void testSetRequestBodyNullThrowsException() {
        try {
            mParameters.setRequestBody(null);
        } catch (NullPointerException e) {
            /* Expected */
        }
    }

    public void testRequestHeadersDefault() {
        assertNotNull(mParameters.getRequestHeadersKeysSet());
    }

    public void testNullRequestHeader() {
        assertNull(mParameters.getHeaderValue(null));
    }

    public void testRequestHeader() {
        String headerName = "HeaderName";
        String headerValue = "HeaderValue";
        mParameters.setRequestHeader(headerName, headerValue);

        assertTrue(mParameters.getRequestHeadersKeysSet().contains(headerName));
        assertEquals(headerValue, mParameters.getHeaderValue(headerName));
    }

    public void testRequestHeaderSetBothNullValue() {
        try {
            mParameters.setRequestHeader(null, null);
        } catch (NullPointerException e) {
            /* Expected */
        }
    }

    public void testRequestHeaderSetNameNull() {
        String headerName = "HeaderName";
        try {
            mParameters.setRequestHeader(headerName, null);
        } catch (NullPointerException e) {
            /* Expected */
        }
    }

    public void testRequestHeaderSetValueNull() {
        String headerValue = "HeaderValue";
        try {
            mParameters.setRequestHeader(null, headerValue);
        } catch (NullPointerException e) {
            /* Expected */
        }
    }

    public void testUriDefault() {
        assertEquals(Uri.EMPTY, mParameters.getUri());
    }

    public void testUriSetNullThrowException() {
        try {
            mParameters.setUri(null);
        } catch (NullPointerException e) {
            /* Expected */
        }
    }

    public void testUri() {
        Uri uri = Uri.parse("http://www.google.com");

        mParameters.setUri(uri);

        assertEquals(uri, mParameters.getUri());
    }
}