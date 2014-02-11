package com.ford.syncV4.android.utils;

import android.os.Looper;
import android.util.Log;

import com.ford.syncV4.android.MainApp;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 1/30/14
 * Time: 2:15 PM
 */
public class AppUtils {

    public static boolean isRunningUIThread() {
        return Looper.myLooper() == Looper.getMainLooper();
    }

    /**
     * Returns the file contents from the specified resource.
     *
     * @param resource Resource id (in res/ directory)
     * @return The resource file's contents
     */
    private byte[] contentsOfResource(int resource) {
        InputStream is = null;
        try {
            is = MainApp.getInstance().getResources().openRawResource(resource);
            ByteArrayOutputStream os = new ByteArrayOutputStream(is.available());
            final int buffersize = 4096;
            final byte[] buffer = new byte[buffersize];
            int available = 0;
            while ((available = is.read(buffer)) >= 0) {
                os.write(buffer, 0, available);
            }
            return os.toByteArray();
        } catch (IOException e) {
            Log.w(TAG, "Can't read icon file", e);
            return null;
        } finally {
            if (is != null) {
                try {
                    is.close();
                } catch (IOException e) {
                    Log.e(TAG, e.toString());
                }
            }
        }
    }
}