package com.batutin.android.androidvideostreaming.reader;

import java.io.ByteArrayOutputStream;
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
        //listener.fileReadWillBegin(this);
        int nRead;
        ByteArrayOutputStream bb = new ByteArrayOutputStream();
        byte[] buffer = new byte[176 * 144];
        while ((nRead = stream.read(buffer, 0, buffer.length)) != -1) {
            //listener.chunkIsReadFromFile(this, buffer);
            bb.write(buffer);
        }

        try {
            bb.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
        byte[] b= bb.toByteArray();

        stream.close();
        //listener.fileReadEnded(this);
        return b;
    }

}
