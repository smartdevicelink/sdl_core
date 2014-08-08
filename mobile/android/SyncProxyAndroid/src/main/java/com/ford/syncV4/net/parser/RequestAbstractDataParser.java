package com.ford.syncV4.net.parser;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 08.08.14
 * Time: 9:41
 */

import com.ford.syncV4.net.HttpRequestParameters;

import org.json.JSONException;

/**
 * This class provides a functionality for the HTTP request data extraction from SDL message
 */
public abstract class RequestAbstractDataParser {

    /**
     * Define JSON data keys
     */
    protected static final String KEY_HTTP_REQUEST = "HTTPRequest";
    protected static final String KEY_HEADERS = "headers";
    protected static final String KEY_CONTENT_TYPE = "ContentType";
    protected static final String KEY_CONNECT_TIMEOUT = "ConnectTimeout";
    protected static final String KEY_DO_OUTPUT = "DoOutput";
    protected static final String KEY_DO_INPUT = "DoInput";
    protected static final String KEY_USE_CACHES = "UseCaches";
    protected static final String KEY_REQUEST_METHOD = "RequestMethod";
    protected static final String KEY_READ_TIMEOUT = "ReadTimeout";
    protected static final String KEY_INSTANCE_FOLLOW_REDIRECTS = "InstanceFollowRedirects";
    protected static final String KEY_CHARSET = "charset";
    public static final String KEY_CONTENT_LENGTH = "Content-Length";
    protected static final String KEY_BODY = "body";

    /**
     * Parse incoming data (bytes array) into {@link com.ford.syncV4.net.HttpRequestParameters}
     *
     * @param incomingData incoming data
     *
     * @return instance of the {@link com.ford.syncV4.net.HttpRequestParameters} object
     */
    public abstract HttpRequestParameters getHTTPRequestParameters(byte[] incomingData) throws JSONException;
}