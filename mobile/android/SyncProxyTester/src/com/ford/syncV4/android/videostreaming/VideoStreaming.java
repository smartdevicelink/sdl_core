package com.ford.syncV4.android.videostreaming;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by admin on 7/19/13.
 */
public class VideoStreaming {

    public byte[] readTestVideoFileFromStream(InputStream stream) throws IOException {
        if (stream == null) throw new IllegalArgumentException("Input stream should not be null");
        int size = stream.available();
        byte[] buffer = new byte[size];
        stream.read(buffer);
        stream.close();
        return buffer;
    }

    public byte[] readTestVideoFileFromStream(InputStream stream, FileStreamReaderListener listener) throws IOException {
        if (stream == null) throw new IllegalArgumentException("Input stream should not be null");
        listener.fileReadWillBegin(this);
        int size = stream.available();
        byte[] buffer = new byte[size];
        stream.read(buffer);
        listener.chunkIsReadFromFile(this, buffer);
        stream.close();
        listener.fileReadEnded(this);
        return buffer;
    }

}
