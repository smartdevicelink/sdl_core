package com.ford.syncV4.net;

import android.net.Uri;

import com.ford.syncV4.util.logger.Logger;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpEntityEnclosingRequestBase;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.ByteArrayEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;

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

    private static final String HEADER_CONTENT_TYPE = "Content-Type";
    private static final String HEADER_CONTENT_LENGTH = "Content-Length";
    private static final String HEADER_CHAR_SET = "charset";

    private static final String HEADER_CONTENT_TYPE_JSON = "application/json";
    private static final String HEADER_CHAR_SET_UTF_8 = "utf-8";

    @Override
    public byte[] downloadDataFromUri(Uri uri, byte[] requestData) {
        if (uri == null) {
            throw new NullPointerException(LOG_TAG + " downloadDataFromUri -> uri is null");
        }
        HttpPost request = new HttpPost(uri.toString());
        request.setHeader(HEADER_CONTENT_TYPE, HEADER_CONTENT_TYPE_JSON);
        request.setHeader(HEADER_CONTENT_LENGTH, String.valueOf(requestData.length));
        request.setHeader(HEADER_CHAR_SET, HEADER_CHAR_SET_UTF_8);
        request.setEntity(new ByteArrayEntity(requestData));
        HttpClient httpClient = new DefaultHttpClient();
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
            Logger.e(LOG_TAG + " ClientProtocolException: " + e.getMessage());
        } catch (IOException e) {
            Logger.e(LOG_TAG + " IOException: " + e.getMessage());
        }
        return new byte[0];
    }
}