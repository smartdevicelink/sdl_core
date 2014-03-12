package com.ford.syncV4.android.manager;

import android.util.SparseArray;

import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.enums.FileType;

import junit.framework.TestCase;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 1/30/14
 * Time: 5:09 PM
 */
public class PutFileTransferManagerTest extends TestCase {

    private static final String LOG_TAG = "PutFileTransferManagerTest";

    private PutFileTransferManager mPutFileTransferManager;
    private String mFileName = "FileName";
    private int mCorrelationId = 123;

    @Override
    protected void setUp() throws Exception {
        super.setUp();

        mPutFileTransferManager = new PutFileTransferManager();
    }

    public void testAddPutFileToAwaitArray() {
        PutFile newPutFile = RPCRequestFactory.buildPutFile();
        newPutFile.setFileType(FileType.AUDIO_MP3);
        newPutFile.setSyncFileName(mFileName);
        newPutFile.setCorrelationID(mCorrelationId);

        mPutFileTransferManager.addPutFileToAwaitArray(mCorrelationId, newPutFile);

        assertTrue(mPutFileTransferManager.hasPutFileInAwaitArray(mCorrelationId));
    }

    public void testRemovePutFileFromAwaitArray() {
        PutFile newPutFile = RPCRequestFactory.buildPutFile();
        newPutFile.setFileType(FileType.AUDIO_MP3);
        newPutFile.setSyncFileName(mFileName);
        newPutFile.setCorrelationID(mCorrelationId);

        mPutFileTransferManager.addPutFileToAwaitArray(mCorrelationId, newPutFile);

        assertTrue(mPutFileTransferManager.hasPutFileInAwaitArray(mCorrelationId));

        mPutFileTransferManager.removePutFileFromAwaitArray(mCorrelationId);

        assertFalse(mPutFileTransferManager.hasPutFileInAwaitArray(mCorrelationId));
    }

    public void testHasNextTrue() {
        PutFile newPutFile = RPCRequestFactory.buildPutFile();
        newPutFile.setFileType(FileType.AUDIO_MP3);
        newPutFile.setSyncFileName(mFileName);
        newPutFile.setCorrelationID(mCorrelationId);

        mPutFileTransferManager.addPutFileToAwaitArray(mCorrelationId, newPutFile);

        assertTrue(mPutFileTransferManager.hasNext());
    }

    public void testHasNextFalse() {
        assertFalse(mPutFileTransferManager.hasNext());
    }

    public void testFlushArray() {
        int arraySize = 10;
        PutFile newPutFile;
        for (int i = 0; i < arraySize; i++) {
            newPutFile = RPCRequestFactory.buildPutFile();
            newPutFile.setFileType(FileType.AUDIO_MP3);
            newPutFile.setSyncFileName(mFileName);
            newPutFile.setCorrelationID(i);
            mPutFileTransferManager.addPutFileToAwaitArray(i, newPutFile);
        }

        while (mPutFileTransferManager.hasNext()) {
            newPutFile = mPutFileTransferManager.getNextPutFile();
            assertNotNull(newPutFile);
        }

        assertFalse(mPutFileTransferManager.hasNext());
    }

    public void testGetCopy() {
        int arraySize = 10;
        PutFile newPutFile;
        for (int i = 0; i < arraySize; i++) {
            newPutFile = RPCRequestFactory.buildPutFile();
            newPutFile.setFileType(FileType.AUDIO_MP3);
            newPutFile.setSyncFileName(mFileName);
            newPutFile.setCorrelationID(i);
            mPutFileTransferManager.addPutFileToAwaitArray(i, newPutFile);
        }

        SparseArray<PutFile> copyArray = mPutFileTransferManager.getCopy();
        mPutFileTransferManager.clear();

        assertFalse(mPutFileTransferManager.hasNext());
        assertEquals(arraySize, copyArray.size());

        PutFile copyPutFile;
        for (int i = 0; i < arraySize; i++) {
            copyPutFile = copyArray.get(copyArray.keyAt(i));
            assertEquals(FileType.AUDIO_MP3, copyPutFile.getFileType());
        }
    }
}