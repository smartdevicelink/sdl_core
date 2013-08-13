package com.batutin.android.androidvideostreaming.media;

import android.media.CamcorderProfile;
import android.media.MediaFormat;

import com.batutin.android.androidvideostreaming.utils.ALog;

import java.nio.ByteBuffer;

public class MediaFormatUtils {

    public static final String MIME_TYPE = "video/avc";

    public static MediaFormat createVideoAvcEncoderMediaFormat(CamcorderProfile camcorderProfile, int colorFormat, int frameRate) {
        MediaFormat mediaFormat = createEncoderMediaFormat(camcorderProfile.videoFrameWidth, camcorderProfile.videoFrameHeight, camcorderProfile.videoBitRate, camcorderProfile.videoFrameRate, colorFormat, frameRate, MIME_TYPE);
        return mediaFormat;
    }

    public static MediaFormat createEncoderMediaFormat(CamcorderProfile camcorderProfile, int colorFormat, int frameRate, String mimeType) {
        MediaFormat mediaFormat = createEncoderMediaFormat(camcorderProfile.videoFrameWidth, camcorderProfile.videoFrameHeight, camcorderProfile.videoBitRate, camcorderProfile.videoFrameRate, colorFormat, frameRate, mimeType);
        return mediaFormat;
    }

    public static MediaFormat createEncoderMediaFormat(int videoFrameWidth, int videoFrameHeight, int videoBitRate, int videoFrameRate, int colorFormat, int frameRate, String mimeType) {
        ALog.d("Start MediaFormat creation");
        MediaFormat mediaFormat = MediaFormat.createVideoFormat(mimeType, videoFrameWidth, videoFrameHeight);
        mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, videoBitRate);
        mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, videoFrameRate);
        mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, colorFormat);
        mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, frameRate);
        ALog.d("MediaFormat " + mediaFormat.toString() + " created");
        return mediaFormat;
    }

    public static MediaFormat createEncoderMediaFormat(EncoderMediaFormatSetting encoderMediaFormatSetting) {
        MediaFormat mediaFormat = createEncoderMediaFormat(encoderMediaFormatSetting.getVideoFrameWidth(), encoderMediaFormatSetting.getVideoFrameHeight(), encoderMediaFormatSetting.getVideoBitRate(), encoderMediaFormatSetting.getVideoFrameRate(), encoderMediaFormatSetting.getColorFormat(), encoderMediaFormatSetting.getFrameRate(), encoderMediaFormatSetting.getMimeType());
        return mediaFormat;
    }

    public static MediaFormat createDecoderMediaFormat(String mimeType, int videoFrameWidth, int videoFrameHeight, ByteBuffer csd0) {
        MediaFormat format =
                MediaFormat.createVideoFormat(mimeType, videoFrameWidth, videoFrameHeight);
        format.setByteBuffer("csd-0", csd0);
        return format;
    }

    public static MediaFormat createVideoAvcDecoderMediaFormat(int videoFrameWidth, int videoFrameHeight, ByteBuffer csd0) {
        MediaFormat format =
                MediaFormat.createVideoFormat(MIME_TYPE, videoFrameWidth, videoFrameHeight);
        format.setByteBuffer("csd-0", csd0);
        return format;
    }


}