package com.ford.syncV4.android.utils;

import android.os.Looper;
import android.util.Log;

import com.ford.syncV4.android.MainApp;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 1/30/14
 * Time: 2:15 PM
 */
public class AppUtils {

    private static final String TAG = "AppUtils";

    public static boolean isRunningUIThread() {
        return Looper.myLooper() == Looper.getMainLooper();
    }

    /**
     * Returns the file contents from the specified resource.
     *
     * @param resource Resource id (in res/ directory)
     * @return         The resource file's contents
     */
    public static byte[] contentsOfResource(int resource) {
        return contentsOfResource(MainApp.getInstance().getResources().openRawResource(resource));
    }

    /**
     * Read a File and return bytes array
     * @param file
     * @return
     */
    public static byte[] contentsOfResource(File file) {
        try {
            return contentsOfResource(new FileInputStream(file));
        } catch (FileNotFoundException e) {
            Log.e(TAG, "Contents Of Resource exception", e);
        }
        return new byte[0];
    }

    public static byte[] contentsOfResource(String fileName) {
        return contentsOfResource(new File(fileName));
    }

    /**
     * Save bytes to a file
     * @param data
     * @param filePath
     * @return
     */
    public static boolean saveDataToFile(byte[] data, String filePath) {
        File mFile = new File(filePath);
        Log.d(TAG, "Saving data to file '" + filePath + "', exists:" + mFile.exists());
        if (mFile.exists()) {
            mFile.delete();
        }
        FileOutputStream mFileOutputStream = null;
        boolean result = false;
        try {
            mFileOutputStream = new FileOutputStream(mFile.getPath());
            mFileOutputStream.write(data);

            result = true;
        } catch (IOException e) {
            Log.e(TAG, "Save Data To File IOException", e);
        } finally {
            if (mFileOutputStream != null) {
                try {
                    mFileOutputStream.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return result;
    }

    /**
     * Read a content of the Input Stream into bytes array
     *
     * @param inputStream a stream of incoming bytes
     * @return            bytes array
     */
    private static byte[] contentsOfResource(InputStream inputStream) {
        try {
            ByteArrayOutputStream byteArrayOutputStream =
                    new ByteArrayOutputStream(inputStream.available());
            final int bufferSize = 4096;
            final byte[] buffer = new byte[bufferSize];
            int available;
            while ((available = inputStream.read(buffer)) >= 0) {
                byteArrayOutputStream.write(buffer, 0, available);
            }
            return byteArrayOutputStream.toByteArray();
        } catch (IOException e) {
            Log.w(TAG, "Can't read file", e);
            return null;
        } finally {
            if (inputStream != null) {
                try {
                    inputStream.close();
                } catch (IOException e) {
                    Log.e(TAG, e.toString());
                }
            }
        }
    }
}