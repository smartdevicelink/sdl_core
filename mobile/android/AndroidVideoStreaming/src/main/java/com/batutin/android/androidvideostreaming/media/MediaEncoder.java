package com.batutin.android.androidvideostreaming.media;

import android.media.MediaCodec;

import com.batutin.android.androidvideostreaming.activity.ALog;

/**
 * Created by Andrew Batutin on 8/9/13.
 */
public class MediaEncoder {

    public static final String MIME_TYPE = "video/avc";
    private MediaCodec encoder;

    public MediaEncoder() {
        encoder = createVideoAvcEncoder();
    }

    public MediaCodec getEncoder() {
        return encoder;
    }

    private MediaCodec createVideoAvcEncoder() {
        ALog.d("Start encoder creation");
        MediaCodec codec = MediaCodec.createEncoderByType(MIME_TYPE);
        ALog.d("End encoder creation");
        return codec;
    }
}
