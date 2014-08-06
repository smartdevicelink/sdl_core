package com.ford.syncV4.service;

import android.net.Uri;

import com.ford.syncV4.net.IDataDownloader;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 06.08.14
 * Time: 12:19
 */

/**
 * This interface provides a methods to download data from network and transform it into suitable
 * format (there is only one format is available now: byte[]).<br>
 * This is a right place to extend functionality of the convert response into different data types.
 */
public interface IDataServiceProvider {

    /**
     * This method provide a possibility to download Policy Table Update data by providing Uri and
     * request body data
     *
     * @param dataDownloader implementation of the {@link com.ford.syncV4.net.IDataDownloader}
     *                       interface
     * @param uri            Uri of the network resource
     * @param requestData    bytes array of the request body
     *
     * @return bytes array
     */
    public byte[] getPolicyTableUpdateData(IDataDownloader dataDownloader, Uri uri,
                                           byte[] requestData);
}