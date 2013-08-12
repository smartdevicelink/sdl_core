package com.batutin.android.androidvideostreaming.media;

import android.media.MediaCodecInfo;

import java.util.Map;

/**
 * Created by Andrew Batutin on 8/9/13.
 */
public class ColorFormatUtilsTest extends MediaUtilsTest {

    private MediaCodecInfo codecInfo;

    public ColorFormatUtilsTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        codecInfo = CodecInfoUtils.selectFirstCodec(MIME_TYPE);
    }

    public void testSelectAvcColorFormatShouldReturnColorFormat() throws Exception {
        int colorFormat = ColorFormatUtils.selectFirstVideoAvcColorFormat();
        assertTrue(colorFormat > 0);
    }

    public void testSelectColorFormatShouldReturnColorFormat() throws Exception {
        int colorFormat = ColorFormatUtils.selectFirstColorFormat(codecInfo.getCapabilitiesForType(MIME_TYPE));
        assertTrue(colorFormat > 0);
    }

    public void testGetColorFormatListShouldBeNotEmpty() throws Exception {
        Map<String, Integer> colorFormatList = ColorFormatUtils.getColorFormatList(codecInfo.getCapabilitiesForType(MIME_TYPE));
        assertNotNull(colorFormatList);
        assertTrue(colorFormatList.size() > 0);
    }

    public void testGetColorSpaceNameShouldReturnValidName() throws Exception {
      String name = ColorFormatUtils.getColorSpaceName(MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar);
        assertEquals(name, ColorFormatUtils.colorFormatNamesMap.get(MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar));
    }

    public void testGetColorSpaceNameShouldReturnUnknown() throws Exception {
        String name = ColorFormatUtils.getColorSpaceName(-1);
        assertEquals("Unknown", name);
    }
}
