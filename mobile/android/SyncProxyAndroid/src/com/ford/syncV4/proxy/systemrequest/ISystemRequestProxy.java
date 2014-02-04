package com.ford.syncV4.proxy.systemrequest;

import com.ford.syncV4.exception.SyncException;
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
     * @throws SyncException if there is an error during preparations to send
     *                       the request
     */
    public void putSystemFile(String filename, byte[] data, FileType fileType)
            throws SyncException;

    /**
     * Uploads a requested part of a downloaded file to the SDL.
     *
     * @param filename the requested file's name
     * @param data     file's contents, in the requested range (the length is
     *                 equal to the array's length)
     * @param offset   the offset at which the data starts
     * @param fileType optional file type
     * @throws SyncException if there is an error during preparations to send
     *                       the request
     */
    public void putSystemFile(String filename, byte[] data, Integer offset,
                              FileType fileType) throws SyncException;
}
