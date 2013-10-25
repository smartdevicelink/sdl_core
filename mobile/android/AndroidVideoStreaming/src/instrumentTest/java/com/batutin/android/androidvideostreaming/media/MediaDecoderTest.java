package com.batutin.android.androidvideostreaming.media;

import android.content.Context;
import android.content.res.AssetManager;
import android.media.MediaFormat;

import java.nio.ByteBuffer;

/**
 * Created by Andrew Batutin on 8/12/13.
 */
public class MediaDecoderTest extends MediaUtilsTest {

    private MediaDecoder sut;
    private MediaFormat format;
    private Context mContext;
    private AssetManager mAssets;

    public MediaDecoderTest() {
        byte[] csd0 = new byte[]{
                0,
                0, 0, 1, 39, 66, 64, 41, -117, -107, 5, -119, -56, 0, 0, 0, 1, 40, -34, 9, -120
        };
        ByteBuffer buffer = ByteBuffer.wrap(csd0);
        format = MediaFormatUtils.createDecoderMediaFormat(MIME_TYPE, VIDEO_FRAME_WIDTH, VIDEO_FRAME_HEIGHT, buffer);
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();

        sut = new MediaDecoder();
        sut.configureMediaDecoder(format, null);
        sut.start();
    }

    @Override
    public void tearDown() throws Exception {
        super.tearDown();
        sut.stop();
    }

    public void testDecoderCreation() throws Exception {
        MediaDecoder decoder = new MediaDecoder();
        assertNotNull(decoder.getDecoder());
    }

    public void testMediaDecoderIsConfigured() throws Exception {
        MediaDecoder decoder = new MediaDecoder();
        decoder.configureMediaDecoder(format, null);
        assertTrue(decoder.isConfigured());
    }

    public void testMediaDecoderIsRunning() throws Exception {
        MediaDecoder decoder = new MediaDecoder();
        decoder.configureMediaDecoder(format, null);
        decoder.start();
        assertTrue(decoder.isRunning());
    }

    public void testMediaDecoderIsStopped() throws Exception {
        MediaDecoder decoder = new MediaDecoder();
        decoder.configureMediaDecoder(format, null);
        decoder.start();
        decoder.stop();
        assertFalse(decoder.isRunning());
    }

    public void testMediaDecoderSecondStartShouldBeIgnored() throws Exception {
        MediaDecoder decoder = new MediaDecoder();
        decoder.configureMediaDecoder(format, null);
        decoder.start();
        decoder.start();
        assertTrue(decoder.isRunning());
    }

    public void testMediaEncoderStopBeforeStartShouldBeIgnored() throws Exception {
        MediaDecoder decoder = new MediaDecoder();
        decoder.configureMediaDecoder(format, null);
        decoder.stop();
        assertFalse(decoder.isRunning());
    }

    public void testMediaEncoderSecondStopShouldBeIgnored() throws Exception {
        MediaDecoder decoder = new MediaDecoder();
        decoder.configureMediaDecoder(format, null);
        decoder.start();
        decoder.stop();
        decoder.stop();
        assertFalse(decoder.isRunning());
    }


}
