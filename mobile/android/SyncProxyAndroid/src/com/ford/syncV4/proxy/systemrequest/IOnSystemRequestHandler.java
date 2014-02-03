package com.ford.syncV4.proxy.systemrequest;

import com.ford.syncV4.proxy.rpc.enums.FileType;

import java.util.List;

/**
 * Interface for handling special OnSystemRequest notifications.
 *
 * Created by enikolsky on 2014-02-03.
 */
public interface IOnSystemRequestHandler {
    /**
     * Requests to download the files from the given URLs. When done, each of
     * the files should be uploaded with the {@link ISystemRequestProxy#putSystemFile(String,
     * byte[], FileType)} method.
     *
     * @param proxy    the proxy used to be upload the downloaded files
     * @param urls     a list of URLs to download files from
     * @param fileType optional file type for HTTP requests
     */
    public void onFilesDownloadRequest(ISystemRequestProxy proxy,
                                       List<String> urls, FileType fileType);
}
