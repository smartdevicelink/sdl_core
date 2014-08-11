package com.ford.syncV4.net;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 07.08.14
 * Time: 17:38
 */

import android.net.Uri;

import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

/**
 * This class contain necessary data for the HTTP Request creation, such as headers, request type,
 * etc ...
 */
public class HttpRequestParameters {

    private static final String LOG_TAG = HttpRequestParameters.class.getSimpleName();

    /**
     * Enumeration of the supported HTTP requests
     */
    public static enum REQUEST_METHOD {
        POST
    }

    /**
     * HTTP request method
     */
    private REQUEST_METHOD mRequestMethod = REQUEST_METHOD.POST;
    /**
     * HTTP Request body
     */
    private JSONObject mRequestBody = new JSONObject();
    /**
     * HTTP Request headers
     */
    private final Map<String, String> mRequestHeaders = new HashMap<String, String>();

    /**
     * Uri of the network resource
     */
    private Uri mUri = Uri.EMPTY;

    /**
     * @return HTTP Request method, default one is
     *         {@link com.ford.syncV4.net.HttpRequestParameters.REQUEST_METHOD#POST}
     */
    public REQUEST_METHOD getRequestMethod() {
        return mRequestMethod;
    }

    /**
     * Set HTTP Request method
     *
     * @param value {@link com.ford.syncV4.net.HttpRequestParameters.REQUEST_METHOD}
     */
    public void setRequestMethod(REQUEST_METHOD value) {
        mRequestMethod = value;
    }

    /**
     * @return HTTP Request body, or null if the one does not specified
     */
    public JSONObject getRequestBody() {
        return mRequestBody;
    }

    /**
     * Set HTTP Request body
     *
     * @param value HTTP Request body as {@link org.json.JSONObject}
     */
    public void setRequestBody(JSONObject value) {
        if (value == null) {
            throw new NullPointerException(LOG_TAG + " set request body -> value is null");
        }
        mRequestBody = value;
    }

    /**
     * @return HTTP Request headers keys set
     */
    public Set<String> getRequestHeadersKeysSet() {
        return mRequestHeaders.keySet();
    }

    /**
     * @param headerName HTTP request header name
     *
     * @return a value of the provided HTTP request header name
     */
    public String getHeaderValue(String headerName) {
        return mRequestHeaders.get(headerName);
    }

    /**
     * Set HTTP Request headers as key - value pairs
     *
     * @param headerName  Name of the header
     * @param headerValue Value of the header
     */
    public void setRequestHeader(String headerName, String headerValue) {
        if (headerName == null) {
            throw new NullPointerException(LOG_TAG + " set request headers -> headerName is null");
        }
        if (headerValue == null) {
            throw new NullPointerException(LOG_TAG + " set request headers -> headerValue is null");
        }
        mRequestHeaders.put(headerName, headerValue);
    }

    /**
     * @return {@link android.net.Uri} of the network resource
     */
    public Uri getUri() {
        return mUri;
    }

    /**
     * Set {@link android.net.Uri} of the network resource
     *
     * @param value {@link android.net.Uri} of the network resource, can not be null
     */
    public void setUri(Uri value) {
        if (value == null) {
            throw new NullPointerException(LOG_TAG + " set Uri -> value is null");
        }
        this.mUri = value;
    }
}