package com.ford.syncV4.net;

import com.ford.syncV4.net.parser.RequestAbstractDataParser;
import com.ford.syncV4.util.logger.Logger;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpEntityEnclosingRequestBase;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.InputStreamEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;

import java.io.ByteArrayInputStream;
import java.io.IOException;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 06.08.14
 * Time: 11:56
 */

/**
 * This is a class that implements {@link com.ford.syncV4.net.IDataDownloader} interface in order
 * to download Policy Table Update data
 */
public class PolicyDataDownloaderImpl implements IDataDownloader {

    private static final String LOG_TAG = PolicyDataDownloaderImpl.class.getSimpleName();

    @Override
    public byte[] downloadDataFromUri(HttpRequestParameters httpRequestParameters) {

        String url = httpRequestParameters.getUri().toString();
        Logger.d(LOG_TAG + " Request url:" + url);

        HttpEntityEnclosingRequestBase request = null;

        if (httpRequestParameters.getRequestMethod() == HttpRequestParameters.REQUEST_METHOD.POST) {
            request = new HttpPost(url);
        }

        if (request == null) {
            Logger.w(LOG_TAG + " downloadDataFromUri -> request is null, " +
                    "method:" + httpRequestParameters.getRequestMethod());
            return new byte[0];
        }

        for (String headerName : httpRequestParameters.getRequestHeadersKeysSet()) {
            // We don't need to set a length of the data as it may be wrong
            if (headerName.equals(RequestAbstractDataParser.KEY_CONTENT_LENGTH)) {
                continue;
            }
            //Logger.d(LOG_TAG + " Request header " +
            //        "'" + headerName + ":" + httpRequestParameters.getHeaderValue(headerName) + "'");
            request.setHeader(headerName, httpRequestParameters.getHeaderValue(headerName));
        }

        HttpClient httpClient = new DefaultHttpClient();
        String jsonString = httpRequestParameters.getRequestBody().toString();
        InputStreamEntity reqEntity =
                new InputStreamEntity(new ByteArrayInputStream(jsonString.getBytes()),
                        jsonString.length());
        reqEntity.setContentType("application/json");
        request.setEntity(reqEntity);

        try {
            HttpResponse httpResponse = httpClient.execute(request);
            Logger.d(LOG_TAG + " Response code: " + httpResponse.getStatusLine().getStatusCode());
            int responseCode = httpResponse.getStatusLine().getStatusCode();
            if (responseCode == 200) {
                HttpEntity entity = httpResponse.getEntity();
                if (entity != null) {
                    try {
                        return EntityUtils.toByteArray(entity);
                    } catch (IOException e) {
                        Logger.e(LOG_TAG + " EntityUtils error: " + e.getMessage());
                    }
                }
            }
        } catch (ClientProtocolException e) {
            Logger.e(LOG_TAG + " ClientProtocolException: " + e);
        } catch (IOException e) {
            Logger.e(LOG_TAG + " IOException: " + e.getMessage());
        }
        return new byte[0];
    }
}