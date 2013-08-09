package com.batutin.android.androidvideostreaming.media;

import android.media.MediaCodecInfo;

import com.batutin.android.androidvideostreaming.activity.ALog;

public class ColorFormatUtils {

    public static int selectColorFormat(MediaCodecInfo codecInfo, String mimeType) throws NullPointerException {
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
}