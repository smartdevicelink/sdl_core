package com.batutin.android.androidvideostreaming.media;

import android.media.CamcorderProfile;
import android.media.MediaFormat;

import java.nio.ByteBuffer;

/**
 * Created by Andrew Batutin on 8/9/13.
 */
public class MediaFormatUtilsTest extends MediaUtilsTest {


    public MediaFormatUtilsTest() {
    }



    public void testEncoderAvcMediaFormatWasCreatedWithRightParameters() throws Exception {
        CamcorderProfile profile = CamcorderProfileUtils.getFirstCameraCamcorderProfile(CamcorderProfile
                .QUALITY_LOW);
        int colorFormat = ColorFormatUtils.selectFirstColorFormat(CodecInfoUtils.selectFirstCodec(MIME_TYPE).getCapabilitiesForType(MIME_TYPE));
        MediaFormat format = MediaFormatUtils.createVideoAvcEncoderMediaFormat(profile, colorFormat, FRAME_INTERVAL);
        assertNotNull(format);
        assertEquals(format.getInteger(MediaFormat.KEY_WIDTH), profile.videoFrameWidth);
        assertEquals(format.getInteger(MediaFormat.KEY_HEIGHT), profile.videoFrameHeight);
        assertEquals(format.getInteger(MediaFormat.KEY_BIT_RATE), profile.videoBitRate);
        assertEquals(format.getInteger(MediaFormat.KEY_FRAME_RATE), profile.videoFrameRate);
        assertEquals(format.getInteger(MediaFormat.KEY_COLOR_FORMAT), colorFormat);
        assertEquals(format.getInteger(MediaFormat.KEY_I_FRAME_INTERVAL), FRAME_INTERVAL);
    }

    public void testEncoderMediaFormatWasCreatedWithRightParameters() throws Exception {
        CamcorderProfile profile = CamcorderProfileUtils.getFirstCameraCamcorderProfile(CamcorderProfile
                .QUALITY_LOW);
        int colorFormat = ColorFormatUtils.selectFirstColorFormat(CodecInfoUtils.selectFirstCodec(MIME_TYPE).getCapabilitiesForType(MIME_TYPE));
        MediaFormat format = MediaFormatUtils.createEncoderMediaFormat(profile, colorFormat, FRAME_INTERVAL, MIME_TYPE);
        assertNotNull(format);
        assertEquals(format.getInteger(MediaFormat.KEY_WIDTH), profile.videoFrameWidth);
        assertEquals(format.getInteger(MediaFormat.KEY_HEIGHT), profile.videoFrameHeight);
        assertEquals(format.getInteger(MediaFormat.KEY_BIT_RATE), profile.videoBitRate);
        assertEquals(format.getInteger(MediaFormat.KEY_FRAME_RATE), profile.videoFrameRate);
        assertEquals(format.getInteger(MediaFormat.KEY_COLOR_FORMAT), colorFormat);
        assertEquals(format.getInteger(MediaFormat.KEY_I_FRAME_INTERVAL), FRAME_INTERVAL);
    }

    public void testEncoderMediaFormatWasCreatedWithExtendedRightParameters() throws Exception {
        int colorFormat = ColorFormatUtils.selectFirstColorFormat(CodecInfoUtils.selectFirstCodec(MIME_TYPE).getCapabilitiesForType(MIME_TYPE));
        MediaFormat format = MediaFormatUtils.createEncoderMediaFormat(VIDEO_FRAME_WIDTH, VIDEO_FRAME_HEIGHT, VIDEO_BIT_RATE, VIDEO_FRAME_RATE, colorFormat, FRAME_INTERVAL, MIME_TYPE);
        assertNotNull(format);
        assertEquals(format.getInteger(MediaFormat.KEY_WIDTH), VIDEO_FRAME_WIDTH);
        assertEquals(format.getInteger(MediaFormat.KEY_HEIGHT), VIDEO_FRAME_HEIGHT);
        assertEquals(format.getInteger(MediaFormat.KEY_BIT_RATE), VIDEO_BIT_RATE);
        assertEquals(format.getInteger(MediaFormat.KEY_FRAME_RATE), VIDEO_FRAME_RATE);
        assertEquals(format.getInteger(MediaFormat.KEY_COLOR_FORMAT), colorFormat);
        assertEquals(format.getInteger(MediaFormat.KEY_I_FRAME_INTERVAL), FRAME_INTERVAL);
    }

    public void testCreateEncoderMediaFormatWithParamObject() throws Exception {
        int colorFormat = ColorFormatUtils.selectFirstColorFormat(CodecInfoUtils.selectFirstCodec(MIME_TYPE).getCapabilitiesForType(MIME_TYPE));
        EncoderMediaFormatSetting settings = new EncoderMediaFormatSetting(VIDEO_FRAME_WIDTH, VIDEO_FRAME_HEIGHT, VIDEO_BIT_RATE, VIDEO_FRAME_RATE, colorFormat, FRAME_INTERVAL, MIME_TYPE);
        MediaFormat format = MediaFormatUtils.createEncoderMediaFormat(settings);
        assertNotNull(format);
        assertEquals(format.getInteger(MediaFormat.KEY_WIDTH), VIDEO_FRAME_WIDTH);
        assertEquals(format.getInteger(MediaFormat.KEY_HEIGHT), VIDEO_FRAME_HEIGHT);
        assertEquals(format.getInteger(MediaFormat.KEY_BIT_RATE), VIDEO_BIT_RATE);
        assertEquals(format.getInteger(MediaFormat.KEY_FRAME_RATE), VIDEO_FRAME_RATE);
        assertEquals(format.getInteger(MediaFormat.KEY_COLOR_FORMAT), colorFormat);
        assertEquals(format.getInteger(MediaFormat.KEY_I_FRAME_INTERVAL), FRAME_INTERVAL);
    }

    public void testCreateDecoderMediaFormatWithCSD0() throws Exception {
        byte[] csd0 = new byte[]{
                0,
                0, 0, 1, 39, 66, 64, 41, -117, -107, 5, -119, -56, 0, 0, 0, 1, 40, -34, 9, -120
        };
        ByteBuffer buffer = ByteBuffer.wrap(csd0);
        MediaFormat format = MediaFormatUtils.createDecoderMediaFormat(MIME_TYPE, VIDEO_FRAME_WIDTH, VIDEO_FRAME_HEIGHT, buffer);
        assertNotNull(format);
        assertEquals(format.getInteger(MediaFormat.KEY_WIDTH), VIDEO_FRAME_WIDTH);
        assertEquals(format.getInteger(MediaFormat.KEY_HEIGHT), VIDEO_FRAME_HEIGHT);
        assertEquals("a huge TODO - figre out the format for csd-0 and csd-1 wich should be related to SPS and PPS. see http://www.cardinalpeak.com/blog/the-h-264-sequence-parameter-set/", buffer, format.getByteBuffer("csd-0"));
    }

    public void testCreateAvcDecoderMediaFormatWithCSD0() throws Exception {
        byte[] csd0 = new byte[]{
                0,
                0, 0, 1, 39, 66, 64, 41, -117, -107, 5, -119, -56, 0, 0, 0, 1, 40, -34, 9, -120
        };
        ByteBuffer buffer = ByteBuffer.wrap(csd0);
        MediaFormat format = MediaFormatUtils.createVideoAvcDecoderMediaFormat( VIDEO_FRAME_WIDTH, VIDEO_FRAME_HEIGHT, buffer);
        assertNotNull(format);
        assertEquals(format.getInteger(MediaFormat.KEY_WIDTH), VIDEO_FRAME_WIDTH);
        assertEquals(format.getInteger(MediaFormat.KEY_HEIGHT), VIDEO_FRAME_HEIGHT);
        assertEquals("a huge TODO - figre out the format for csd-0 and csd-1 wich should be related to SPS and PPS. see http://www.cardinalpeak.com/blog/the-h-264-sequence-parameter-set/", buffer, format.getByteBuffer("csd-0"));
    }
}
