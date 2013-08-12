package com.batutin.android.androidvideostreaming.media;

import android.media.MediaFormat;

/**
 * Created by Andrew Batutin on 8/9/13.
 */
public class MediaEncoderTest extends MediaUtilsTest {

    private MediaEncoder sut;
    private int colorFormat = ColorFormatUtils.selectFirstColorFormat(CodecInfoUtils.selectFirstCodec(MIME_TYPE).getCapabilitiesForType(MIME_TYPE));
    private MediaFormatSetting settings = new MediaFormatSetting(VIDEO_FRAME_WIDTH, VIDEO_FRAME_HEIGHT, VIDEO_BIT_RATE, VIDEO_FRAME_RATE, colorFormat, FRAME_INTERVAL, MIME_TYPE);

    public MediaEncoderTest() {

    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        sut = new MediaEncoder();
        MediaFormat format = MediaFormatUtils.createMediaFormat(settings);
        sut.configureMediaEncoder(format);
        sut.start();
    }

    @Override
    public void tearDown() throws Exception {
        super.tearDown();
        sut.stop();
    }

    public void testEncoderCreation() throws Exception {
        MediaEncoder encoder = new MediaEncoder();
        assertNotNull(encoder.getEncoder());
    }

    public void testMediaEncoderIsConfigured() throws Exception {
        MediaFormat format = MediaFormatUtils.createMediaFormat(settings);
        MediaEncoder encoder = new MediaEncoder();
        encoder.configureMediaEncoder(format);
        assertTrue(encoder.isConfigured());
    }

    public void testMediaEncoderIsRunning() throws Exception {
        MediaFormat format = MediaFormatUtils.createMediaFormat(settings);
        MediaEncoder encoder = new MediaEncoder();
        encoder.configureMediaEncoder(format);
        encoder.start();
        assertTrue(encoder.isRunning());
    }

    public void testMediaEncoderIsStopped() throws Exception {
        MediaFormat format = MediaFormatUtils.createMediaFormat(settings);
        MediaEncoder encoder = new MediaEncoder();
        encoder.configureMediaEncoder(format);
        encoder.start();
        encoder.stop();
        assertFalse(encoder.isRunning());
    }

    public void testMediaEncoderSecondStartShouldBeIgnored() throws Exception {
        MediaFormat format = MediaFormatUtils.createMediaFormat(settings);
        MediaEncoder encoder = new MediaEncoder();
        encoder.configureMediaEncoder(format);
        encoder.start();
        encoder.start();
        assertTrue(encoder.isRunning());
    }

    public void testMediaEncoderStopBeforeStartShouldBeIgnored() throws Exception {
        MediaFormat format = MediaFormatUtils.createMediaFormat(settings);
        MediaEncoder encoder = new MediaEncoder();
        encoder.configureMediaEncoder(format);
        encoder.stop();
        assertFalse(encoder.isRunning());
    }

    public void testMediaEncoderSecondStopShouldBeIgnored() throws Exception {
        MediaFormat format = MediaFormatUtils.createMediaFormat(settings);
        MediaEncoder encoder = new MediaEncoder();
        encoder.configureMediaEncoder(format);
        encoder.start();
        encoder.stop();
        encoder.stop();
        assertFalse(encoder.isRunning());
    }
}
