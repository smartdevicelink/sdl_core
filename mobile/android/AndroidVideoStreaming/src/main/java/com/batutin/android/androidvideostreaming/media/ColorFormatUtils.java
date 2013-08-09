package com.batutin.android.androidvideostreaming.media;

import android.media.MediaCodecInfo;

import com.batutin.android.androidvideostreaming.activity.ALog;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public class ColorFormatUtils {


    public static final Map<Integer, String> colorFormatNamesMap;

    static {
        Map<Integer, String> aMap = new HashMap<Integer, String>();
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatMonochrome, "COLOR_FormatMonochrome");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format8bitRGB332, "COLOR_Format8bitRGB332");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format12bitRGB444, "COLOR_Format12bitRGB444");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format16bitARGB4444, "COLOR_Format16bitARGB4444");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format16bitARGB1555, "COLOR_Format16bitARGB1555");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format16bitRGB565, "COLOR_Format16bitRGB565");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format16bitBGR565, "COLOR_Format16bitBGR565");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format18bitRGB666, "COLOR_Format18bitRGB666");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format18bitARGB1665, "COLOR_Format18bitARGB1665");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format19bitARGB1666, "COLOR_Format19bitARGB1666");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format24bitRGB888, "COLOR_Format24bitRGB888");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format24bitBGR888, "COLOR_Format24bitBGR888");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format24bitARGB1887, "COLOR_Format24bitARGB1887");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format25bitARGB1888, "COLOR_Format25bitARGB1888");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format32bitBGRA8888, "COLOR_Format32bitBGRA8888");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format32bitARGB8888, "COLOR_Format32bitARGB8888");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV411Planar, "COLOR_FormatYUV411Planar");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV411PackedPlanar, "COLOR_FormatYUV411PackedPlanar");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar, "COLOR_FormatYUV420Planar");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420PackedPlanar, "COLOR_FormatYUV420PackedPlanar");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar, "COLOR_FormatYUV420SemiPlanar");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV422Planar, "COLOR_FormatYUV422Planar");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV422PackedPlanar, "COLOR_FormatYUV422PackedPlanar");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV422SemiPlanar, "COLOR_FormatYUV422SemiPlanar");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatYCbYCr, "COLOR_FormatYCbYCr");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatYCrYCb, "COLOR_FormatYCrYCb");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatCbYCrY, "COLOR_FormatCbYCrY");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatCrYCbY, "COLOR_FormatCrYCbY");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV444Interleaved, "COLOR_FormatYUV444Interleaved");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatRawBayer8bit, "COLOR_FormatRawBayer8bit");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatRawBayer10bit, "COLOR_FormatRawBayer10bit");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatRawBayer8bitcompressed, "COLOR_FormatRawBayer8bitcompressed");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatL2, "COLOR_FormatL2");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatL4, "COLOR_FormatL4");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatL8, "COLOR_FormatL8");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatL16, "COLOR_FormatL16");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatL24, "COLOR_FormatL24");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatL32, "COLOR_FormatL32");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420PackedSemiPlanar, "COLOR_FormatYUV420PackedSemiPlanar");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV422PackedSemiPlanar, "COLOR_FormatYUV422PackedSemiPlanar");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format18BitBGR666, "COLOR_Format18BitBGR666");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format24BitARGB6666, "COLOR_Format24BitARGB6666");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_Format24BitABGR6666, "COLOR_Format24BitABGR6666");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_TI_FormatYUV420PackedSemiPlanar, "COLOR_TI_FormatYUV420PackedSemiPlanar");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_FormatSurface, "COLOR_FormatSurface");
        aMap.put(MediaCodecInfo.CodecCapabilities.COLOR_QCOM_FormatYUV420SemiPlanar, "COLOR_QCOM_FormatYUV420SemiPlanar");
        colorFormatNamesMap = Collections.unmodifiableMap(aMap);
    }

    public static int selectFirstColorFormat(MediaCodecInfo codecInfo, String mimeType) throws NullPointerException {
        ALog.d("Start color format selecting");
        MediaCodecInfo.CodecCapabilities capabilities = codecInfo.getCapabilitiesForType(mimeType);
        if (capabilities.colorFormats == null || capabilities.colorFormats.length == 0) {
            ALog.e("Color format not found");
            throw new NullPointerException("Unable to get color formats");
        }
        for (int i = 0; i < capabilities.colorFormats.length; i++) {
            int colorFormat = capabilities.colorFormats[i];
            ALog.d("Color format " + colorFormat + " selected");
            return colorFormat;
        }
        return 0;   // not reached
    }

    public static Map<String, Integer> getColorFormatList(MediaCodecInfo.CodecCapabilities capabilities) {
        Map<String, Integer> coorFormatList = new HashMap<String, Integer>();
        for (int i = 0; i < capabilities.colorFormats.length; i++) {
            int colorFormat = capabilities.colorFormats[i];
            coorFormatList.put(getColorSpaceName(colorFormat), colorFormat);
        }
        return coorFormatList;
    }

    private static String getColorSpaceName(int colorFormat) {
        if (colorFormatNamesMap.containsKey(colorFormat) == false) {
            return "Unknown";
        } else {
            return colorFormatNamesMap.get(colorFormat);
        }

    }

}