package com.batutin.android.androidvideostreaming.media;

import android.media.MediaCodecInfo;

import java.util.List;

/**
 * Created by Andrew Batutin on 8/9/13.
 */
public class CodecInfoUtilsTest extends MediaUtilsTest {

    public CodecInfoUtilsTest() {
    }

    public void testSelectCodecShouldReturnCodec() throws Exception {
        MediaCodecInfo info = CodecInfoUtils.selectFirstCodec(MIME_TYPE);
        assertNotNull(info);
    }

    public void testSelectCodecShouldReturnListOfCodecsInfo() throws Exception {
        List<MediaCodecInfo> infoList = CodecInfoUtils.getSupportedMediaCodecInfoList(MIME_TYPE);
        assertNotNull(infoList);
        assertTrue(infoList.size() > 0);
    }

    public void testSelectCodecShouldThroughException() throws Exception {
        try {
            MediaCodecInfo info = CodecInfoUtils.selectFirstCodec("wrong_type");
            assertNull("should not get here", info);
        } catch (IllegalArgumentException e) {
            assertNotNull(e);
        }
    }
}
