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

    public InputStream readFileFromAssets(String fileName) {
        InputStream inputStream = null;
        try {
            inputStream = getInputStreamForFile(fileName);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return inputStream;
    }

    private InputStream getInputStreamForFile(String fileName) throws IOException {
        if (this.getContext() == null)
            throw new IllegalArgumentException("Context should be initialised before using this method");
        InputStream stream = this.getContext().getAssets().open(fileName);
        return stream;
    }

}
