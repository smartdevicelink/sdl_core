package com.batutin.android.androidvideostreaming.test.reader;

import android.content.res.AssetManager;
import android.test.AndroidTestCase;

import junit.framework.Assert;

import java.io.InputStream;

import com.batutin.android.androidvideostreaming.reader.FileStreamReaderListener;
import com.batutin.android.androidvideostreaming.reader.VideoStreaming;

/**
 * Created by admin on 7/19/13.
 */
public class VideoStreamingTestCase extends AndroidTestCase {

    public static final String FILE_NAME = "test_video.mp4";
    private VideoStreaming sut;
    private InputStream inputStream;

    public VideoStreamingTestCase() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        sut = new VideoStreaming();
        AssetManager aManager = this.getContext().getAssets();
        inputStream = aManager.open(FILE_NAME);
    }

    public void testReadTestVideoFileFromStreamNotNull() throws Exception {
        AssetManager aManager = this.getContext().getAssets();
        InputStream stream = aManager.open(FILE_NAME);
        byte[] bytes =  sut.readTestVideoFileFromStream(stream);
        assertNotNull(bytes);
    }

    public void testReadFileWithNullStreamShouldThrowsException() throws Exception {
        try {
            sut.readTestVideoFileFromStream(null);
            Assert.fail("read with null stream should throws illegal arg exception");
        }catch (IllegalArgumentException e){}
    }

    public void testReadFileWithChunksShouldGiveChunks() throws Exception {
        sut.readTestVideoFileFromStream(this.inputStream, new FileStreamReaderListener() {
            @Override
            public void fileReadWillBegin(VideoStreaming source) {
                assertEquals(VideoStreamingTestCase.this.sut, source);
            }

            @Override
            public void chunkIsReadFromFile(VideoStreaming source, byte[] chunk) {
                assertEquals(VideoStreamingTestCase.this.sut, source);
                assertNotNull(chunk);
            }

            @Override
            public void fileReadEnded(VideoStreaming source) {
                assertEquals(VideoStreamingTestCase.this.sut, source);
            }
        });
    }
}
