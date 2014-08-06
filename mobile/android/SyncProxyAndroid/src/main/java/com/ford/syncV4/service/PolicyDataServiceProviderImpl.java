package com.ford.syncV4.service;

import android.net.Uri;

import com.ford.syncV4.net.IDataDownloader;

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

    @Override
    public byte[] getPolicyTableUpdateData(IDataDownloader dataDownloader, Uri uri,
                                           byte[] requestData) {
        if (dataDownloader == null) {
            throw new NullPointerException(LOG_TAG + " getPolicyTableUpdateData -> " +
                    "downloader is null");
        }
        return dataDownloader.downloadDataFromUri(uri, requestData);
    }
}