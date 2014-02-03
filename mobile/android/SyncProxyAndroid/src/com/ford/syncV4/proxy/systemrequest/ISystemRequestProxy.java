package com.ford.syncV4.proxy.systemrequest;

import com.ford.syncV4.proxy.rpc.enums.FileType;

/**
 * Interface to communicate back to the SDL about special OnSystemRequest
 * notifications.
 *
 * Created by enikolsky on 2014-02-03.
 */
public interface ISystemRequestProxy {
    /**
     * Uploads a requested downloaded file to the SDL.
     *
     * @param filename filename as received from the server
     * @param data     file's contents
     * @param fileType optional file type
     */
    public void putSystemFile(String filename, byte[] data, FileType fileType);
}
