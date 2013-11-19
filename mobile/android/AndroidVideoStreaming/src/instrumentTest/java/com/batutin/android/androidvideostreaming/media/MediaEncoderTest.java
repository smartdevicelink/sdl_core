package com.batutin.android.androidvideostreaming.media;

import android.media.MediaFormat;

/**
 * Created by Andrew Batutin on 8/9/13.
 */
public class MediaEncoderTest extends MediaUtilsTest {

    protected int colorFormat = ColorFormatUtils.selectFirstColorFormat(CodecInfoUtils.selectFirstCodec(MIME_TYPE).getCapabilitiesForType(MIME_TYPE));
    protected EncoderMediaFormatSetting encoderSettings = new EncoderMediaFormatSetting(VIDEO_FRAME_WIDTH, VIDEO_FRAME_HEIGHT, VIDEO_BIT_RATE, VIDEO_FRAME_RATE, colorFormat, FRAME_INTERVAL, MIME_TYPE);
    private MediaEncoder sut;

    public MediaEncoderTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        sut = new MediaEncoder();
        MediaFormat format = MediaFormatUtils.createEncoderMediaFormat(encoderSettings);
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
        MediaFormat format = MediaFormatUtils.createEncoderMediaFormat(encoderSettings);
        MediaEncoder encoder = new MediaEncoder();
        encoder.configureMediaEncoder(format);
        assertTrue(encoder.isConfigured());
    }

    public void testMediaEncoderIsRunning() throws Exception {
        MediaFormat format = MediaFormatUtils.createEncoderMediaFormat(encoderSettings);
        MediaEncoder encoder = new MediaEncoder();
        encoder.configureMediaEncoder(format);
        encoder.start();
        assertTrue(encoder.isRunning());
    }

    public void testMediaEncoderIsStopped() throws Exception {
        MediaFormat format = MediaFormatUtils.createEncoderMediaFormat(encoderSettings);
        MediaEncoder encoder = new MediaEncoder();
        encoder.configureMediaEncoder(format);
        encoder.start();
        encoder.stop();
        assertFalse(encoder.isRunning());
    }

    public void testMediaEncoderSecondStartShouldBeIgnored() throws Exception {
        MediaFormat format = MediaFormatUtils.createEncoderMediaFormat(encoderSettings);
        MediaEncoder encoder = new MediaEncoder();
        encoder.configureMediaEncoder(format);
        encoder.start();
        encoder.start();
        assertTrue(encoder.isRunning());
    }

    public void testMediaEncoderStopBeforeStartShouldBeIgnored() throws Exception {
        MediaFormat format = MediaFormatUtils.createEncoderMediaFormat(encoderSettings);
        MediaEncoder encoder = new MediaEncoder();
        encoder.configureMediaEncoder(format);
        encoder.stop();
        assertFalse(encoder.isRunning());
    }

    public void testMediaEncoderSecondStopShouldBeIgnored() throws Exception {
        MediaFormat format = MediaFormatUtils.createEncoderMediaFormat(encoderSettings);
        MediaEncoder encoder = new MediaEncoder();
        encoder.configureMediaEncoder(format);
        encoder.start();
        encoder.stop();
        encoder.stop();
        assertFalse(encoder.isRunning());
    }

    public void testMediaEncoderCreationWithWrongFrameSizeThrowsException() throws Exception {
        EncoderMediaFormatSetting settings = new EncoderMediaFormatSetting(100, 100, VIDEO_BIT_RATE, VIDEO_FRAME_RATE, colorFormat, FRAME_INTERVAL, MIME_TYPE);
        MediaFormat format = MediaFormatUtils.createEncoderMediaFormat(settings);
        try {
            MediaEncoder encoder = new MediaEncoder();
            encoder.configureMediaEncoder(format);
            encoder.start();
            assertFalse("Only CamcorderProfile.QUALITY_LOW  and CamcorderProfile.QUALITY_HIGH frame sizes are guaranteed to work, should not get here", true);
        } catch (IllegalStateException e) {
            assertNotNull("Only CamcorderProfile.QUALITY_LOW  and CamcorderProfile.QUALITY_HIGH frame sizes are guaranteed to work", e);
        }

    }

    public void testMediaEncoderCreationWithRightFrameSize() throws Exception {
        EncoderMediaFormatSetting settings = new EncoderMediaFormatSetting(192, 208, VIDEO_BIT_RATE, VIDEO_FRAME_RATE, colorFormat, FRAME_INTERVAL, MIME_TYPE);
        MediaFormat format = MediaFormatUtils.createEncoderMediaFormat(settings);
        try {
            MediaEncoder encoder = new MediaEncoder();
            encoder.configureMediaEncoder(format);
            encoder.start();
            assertTrue(encoder.isRunning());
        } catch (IllegalStateException e) {
            assertFalse("Frame size should be multiple of 16, but sizes << CamcorderProfile.QUALITY_LOW and >> CamcorderProfile.QUALITY_HIGH may not work, should not get here", true);
        }

    }
}
