package com.ford.syncV4.net;

import android.net.Uri;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 06.08.14
 * Time: 11:48
 */

/**
 * This interface provides a functionality to download data from the network
 */
public interface IDataDownloader {

    /**
     * Method to download data from network resources by specifying Uri and HTTP requests
     * body data
     *
     * @param uri         {@link android.net.Uri} of the network resource
     * @param requestData data of the HTTP requests body
     *
     * @return response from the network resource as array of the bytes filled by the data if success
     * or empty array of the bytes if any fail occur
     */
    public byte[] downloadDataFromUri(Uri uri, byte[] requestData);
}