package com.ford.syncV4.net.parser;

import com.ford.syncV4.net.HttpRequestParameters;
import com.ford.syncV4.util.StringUtils;
import com.ford.syncV4.util.logger.Logger;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Iterator;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 08.08.14
 * Time: 9:52
 */

/**
 * This class is providing functionality to parse incoming from SDL data into
 * {@link com.ford.syncV4.net.HttpRequestParameters} assuming that data of the request is
 * providing in the JSON format
 */
public class RequestJSONDataParser extends RequestAbstractDataParser {

    @SuppressWarnings("unused")
    protected static final String LOG_TAG = RequestJSONDataParser.class.getSimpleName();

    @Override
    public HttpRequestParameters getHTTPRequestParameters(byte[] incomingData) throws JSONException {
        HttpRequestParameters httpRequestParameters = new HttpRequestParameters();
        JSONObject jsonObject = getJSONFromRawData(incomingData);

        if (!jsonObject.has(KEY_HTTP_REQUEST)) {
            Logger.w(LOG_TAG + " getHTTPRequestParameters there is no " + KEY_HTTP_REQUEST + " key");
            return httpRequestParameters;
        }

        JSONObject httpRequest = jsonObject.getJSONObject(KEY_HTTP_REQUEST);

        if (!httpRequest.has(KEY_HEADERS)) {
            Logger.w(LOG_TAG + " getHTTPRequestParameters there is no " + KEY_HEADERS + " key");
            return httpRequestParameters;
        }

        parseHeaders(httpRequestParameters, httpRequest.getJSONObject(KEY_HEADERS));

        if (!httpRequest.has(KEY_BODY)) {
            Logger.w(LOG_TAG + " getHTTPRequestParameters there is no " + KEY_BODY + " key");
            return httpRequestParameters;
        }

        httpRequestParameters.setRequestBody(new JSONObject(httpRequest.getString(KEY_BODY)));

        return httpRequestParameters;
    }

    /**
     * Convert raw data into {@link org.json.JSONObject}
     *
     * @param rawData raw data as bytes array object which is extracted from the SDLs message
     *
     * @return a {@link org.json.JSONObject} converted from bytes array
     */
    protected static JSONObject getJSONFromRawData(byte[] rawData) {
        JSONObject jsonObject = new JSONObject();
        if (rawData == null) {
            Logger.w(LOG_TAG + " Raw data is null");
            return jsonObject;
        }

        if (rawData.length == 0) {
            Logger.w(LOG_TAG + " Raw data is empty");
            return jsonObject;
        }

        try {
            String string = getStringFromBytesArray(rawData);
            jsonObject = new JSONObject(string);
        } catch (JSONException e) {
            Logger.e(LOG_TAG + " Create JSON error:" + e.getMessage());
        }
        return jsonObject;
    }

    /**
     * Convert {@link org.json.JSONObject} into {@link org.json.JSONArray}
     *
     * @param jsonObject input data as {@link org.json.JSONObject}
     *
     * @return a {@link org.json.JSONArray} of the headers
     */
    protected void parseHeaders(HttpRequestParameters httpRequestParameters,
                                     JSONObject jsonObject) throws JSONException {
        final Iterator<String> iterator = jsonObject.keys();
        String headerName;
        while (iterator.hasNext()) {
            headerName = iterator.next();
            if (StringUtils.isEmpty(headerName)) {
                continue;
            }
            httpRequestParameters.setRequestHeader(headerName, jsonObject.getString(headerName));
        }
    }

    /**
     * Convert bytes array into String
     *
     * @param bytesArray raw data as bytes array object
     *
     * @return a String converted from bytes array, an empty String is the incoming data is in
     *         incorrect format (for example a null) or is the is an error occur
     */
    private static String getStringFromBytesArray(byte[] bytesArray) {
        if (bytesArray == null) {
            return "";
        }
        return new String(bytesArray);
    }
}