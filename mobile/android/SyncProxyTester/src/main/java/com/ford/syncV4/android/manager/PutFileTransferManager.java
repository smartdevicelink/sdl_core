package com.ford.syncV4.android.manager;

import android.util.SparseArray;

import com.ford.syncV4.proxy.rpc.PutFile;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 1/30/14
 * Time: 4:30 PM
 */
public class PutFileTransferManager {

    // A map to track PutFiles which are send successfully
    private SparseArray<PutFile> mPutFilesArray;

    public PutFileTransferManager() {
        mPutFilesArray = new SparseArray<PutFile>();
    }

    /**
     * Keep PutFile in the array
     *
     * @param correlationId unique identifier of the PutFile object
     * @param putFile PutFile object to store in array
     */
    public void addPutFileToAwaitArray(int correlationId, PutFile putFile) {
        mPutFilesArray.put(correlationId, putFile);
    }

    /**
     * Remove PutFile from the array
     * @param correlationId unique identifier of the PutFile object
     */
    public void removePutFileFromAwaitArray(int correlationId) {
        PutFile putFile = mPutFilesArray.get(correlationId);
        if (putFile != null) {

            // Not sure if this technique will actually null an Object
            addPutFileToAwaitArray(correlationId, null);

            mPutFilesArray.delete(correlationId);
        }
    }

    /**
     * Checks whether a PutFile with given unique identifier exists in the array
     * @param correlationId unique identifier of the PutFile object
     * @return true if PutFile with such correlationId exists, false in any other case
     */
    public boolean hasPutFileInAwaitArray(int correlationId) {
        return mPutFilesArray != null && mPutFilesArray.get(correlationId) != null;
    }

    /**
     * Return and remove next available PutFile object from array
     * @return PutFile object if such exists or null if array is empty
     */
    public PutFile getNextPutFile() {
        if (mPutFilesArray.size() == 0) {
            return null;
        }
        PutFile putFile = mPutFilesArray.get(mPutFilesArray.size() - 1);
        mPutFilesArray.delete(mPutFilesArray.size() - 1);
        return putFile;
    }

    /**
     * Check if there are any PutFiles in array
     * @return true if there are PutFiles in array, false if array is empty
     */
    public boolean hasNext() {
        return mPutFilesArray != null && mPutFilesArray.size() > 0;
    }

    /**
     * Return a copy of the array
     * @return a copy of the array
     */
    public SparseArray<PutFile> getCopy() {
        // TODO : Cover with tests
        return mPutFilesArray.clone();
    }

    /**
     * Clear the existed array of PutFiles
     */
    public void clear() {
        // TODO : Cover with tests
        if (mPutFilesArray != null) {
            mPutFilesArray.clear();
        }
    }
}