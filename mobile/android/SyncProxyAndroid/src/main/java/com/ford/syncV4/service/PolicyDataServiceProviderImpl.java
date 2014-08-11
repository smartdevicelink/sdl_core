package com.ford.syncV4.service;

import android.net.Uri;

import com.ford.syncV4.net.HttpRequestParameters;
import com.ford.syncV4.net.IDataDownloader;
import com.ford.syncV4.net.parser.RequestAbstractDataParser;
import com.ford.syncV4.util.StringUtils;
import com.ford.syncV4.util.logger.Logger;

import org.json.JSONException;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 06.08.14
 * Time: 12:30
 */

/**
 * This class is an implementation of the {@link com.ford.syncV4.service.IDataServiceProvider}
 * interface and allows to download Policy Table Update data and transfer it to various data formats.
 * Up to now there is only byte[] is necessary.
 */
public class PolicyDataServiceProviderImpl implements IDataServiceProvider {

    private static final String LOG_TAG = PolicyDataServiceProviderImpl.class.getSimpleName();

    private RequestAbstractDataParser mDataParser;

    public PolicyDataServiceProviderImpl(RequestAbstractDataParser dataParser) {
        if (dataParser == null) {
            throw new NullPointerException(LOG_TAG + " Constructor -> data parser is null");
        }
        mDataParser = dataParser;
    }

    @Override
    public byte[] getPolicyTableUpdateData(IDataDownloader dataDownloader, String url,
                                           byte[] incomingData) {
        if (dataDownloader == null) {
            throw new NullPointerException(LOG_TAG + " getPolicyTableUpdateData -> " +
                    "downloader is null");
        }

        HttpRequestParameters parameters;
        byte[] responseData = new byte[0];

        try {
            parameters = mDataParser.getHTTPRequestParameters(incomingData);
        } catch (JSONException e) {
            Logger.e(LOG_TAG + " getPolicyTableUpdateData:" + e.getMessage());

            return responseData;
        }

        if (StringUtils.isEmpty(parameters.getUri().toString())) {
            parameters.setUri(Uri.parse(url));
        }

        return dataDownloader.downloadDataFromUri(parameters);
    }
}