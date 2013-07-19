package com.ford.syncV4.android.videostreaming;

import android.content.Context;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by admin on 7/19/13.
 */
public class AssetsReader {

    private Context context;
    private VideoStreaming videoStreaming;

    public AssetsReader(Context context) {
        if (context == null)
            throw new IllegalArgumentException("Be initialised with null context object should not.  Yeesssssss.");
        this.setContext(context);
        videoStreaming = new VideoStreaming();
    }

    public Context getContext() {
        return context;
    }

    public void setContext(Context context) {
        this.context = context;
    }

    public byte[] readFileFromAssets(String fileName) {
        byte[] buffer = null;
        try {
            buffer = getBytesFromStream(fileName);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return buffer;
    }

    private byte[] getBytesFromStream(String fileName) throws IOException {
        InputStream stream = this.getInputStreamForFile(fileName);
        byte[] buffer = videoStreaming.readTestVideoFileFromStream(stream);
        return buffer;
    }

    private InputStream getInputStreamForFile(String fileName) throws IOException {
        if (this.getContext() == null)
            throw new IllegalArgumentException("Context should be initialised before using this method");
        InputStream stream = this.getContext().getAssets().open(fileName);
        return stream;
    }

}
