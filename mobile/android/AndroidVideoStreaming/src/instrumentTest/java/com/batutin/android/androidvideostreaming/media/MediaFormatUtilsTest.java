package com.batutin.android.androidvideostreaming.media;

import android.media.CamcorderProfile;
import android.media.MediaFormat;

/**
 * Created by Andrew Batutin on 8/9/13.
 */
public class MediaFormatUtilsTest extends MediaUtilsTest {


    public MediaFormatUtilsTest() {
    }

    public void testMediaFormatWasCreatedWithRightParameters() throws Exception {
        CamcorderProfile profile = CamcorderProfileUtils.getFirstCameraCamcorderProfile(CamcorderProfile
                .QUALITY_LOW);
        int colorFormat = ColorFormatUtils.selectFirstColorFormat(CodecInfoUtils.selectFirstCodec(MIME_TYPE).getCapabilitiesForType(MIME_TYPE));
        MediaFormat format = MediaFormatUtils.createMediaFormat(profile, colorFormat, FRAME_INTERVAL, MIME_TYPE);
        assertNotNull(format);
        assertEquals(format.getInteger(MediaFormat.KEY_WIDTH), profile.videoFrameWidth);
        assertEquals(format.getInteger(MediaFormat.KEY_HEIGHT), profile.videoFrameHeight);
        assertEquals(format.getInteger(MediaFormat.KEY_BIT_RATE), profile.videoBitRate);
        assertEquals(format.getInteger(MediaFormat.KEY_FRAME_RATE), profile.videoFrameRate);
        assertEquals(format.getInteger(MediaFormat.KEY_COLOR_FORMAT), colorFormat);
        assertEquals(format.getInteger(MediaFormat.KEY_I_FRAME_INTERVAL), FRAME_INTERVAL);
    }

    public void testMediaFormatWasCreatedWithExtendedRightParameters() throws Exception {
        int colorFormat = ColorFormatUtils.selectFirstColorFormat(CodecInfoUtils.selectFirstCodec(MIME_TYPE).getCapabilitiesForType(MIME_TYPE));
        MediaFormat format = MediaFormatUtils.createMediaFormat(VIDEO_FRAME_WIDTH, VIDEO_FRAME_HEIGHT, VIDEO_BIT_RATE, VIDEO_FRAME_RATE, colorFormat, FRAME_INTERVAL, MIME_TYPE);
        assertNotNull(format);
        assertEquals(format.getInteger(MediaFormat.KEY_WIDTH), VIDEO_FRAME_WIDTH);
        assertEquals(format.getInteger(MediaFormat.KEY_HEIGHT), VIDEO_FRAME_HEIGHT);
        assertEquals(format.getInteger(MediaFormat.KEY_BIT_RATE), VIDEO_BIT_RATE);
        assertEquals(format.getInteger(MediaFormat.KEY_FRAME_RATE), VIDEO_FRAME_RATE);
        assertEquals(format.getInteger(MediaFormat.KEY_COLOR_FORMAT), colorFormat);
        assertEquals(format.getInteger(MediaFormat.KEY_I_FRAME_INTERVAL), FRAME_INTERVAL);
    }

    public void testCreateMediaFormatWithParamObject() throws Exception {
        int colorFormat = ColorFormatUtils.selectFirstColorFormat(CodecInfoUtils.selectFirstCodec(MIME_TYPE).getCapabilitiesForType(MIME_TYPE));
        MediaFormatSetting settings = new MediaFormatSetting(VIDEO_FRAME_WIDTH, VIDEO_FRAME_HEIGHT, VIDEO_BIT_RATE, VIDEO_FRAME_RATE, colorFormat, FRAME_INTERVAL, MIME_TYPE);
        MediaFormat format = MediaFormatUtils.createMediaFormat(settings);
        assertNotNull(format);
        assertEquals(format.getInteger(MediaFormat.KEY_WIDTH), VIDEO_FRAME_WIDTH);
        assertEquals(format.getInteger(MediaFormat.KEY_HEIGHT), VIDEO_FRAME_HEIGHT);
        assertEquals(format.getInteger(MediaFormat.KEY_BIT_RATE), VIDEO_BIT_RATE);
        assertEquals(format.getInteger(MediaFormat.KEY_FRAME_RATE), VIDEO_FRAME_RATE);
        assertEquals(format.getInteger(MediaFormat.KEY_COLOR_FORMAT), colorFormat);
        assertEquals(format.getInteger(MediaFormat.KEY_I_FRAME_INTERVAL), FRAME_INTERVAL);
    }
}
