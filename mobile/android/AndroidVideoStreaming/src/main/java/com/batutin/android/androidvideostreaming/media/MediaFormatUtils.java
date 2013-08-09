package com.batutin.android.androidvideostreaming.media;

import android.media.CamcorderProfile;
import android.media.MediaFormat;

public class MediaFormatUtils {

    public static MediaFormat createMediaFormat(CamcorderProfile camcorderProfile, int colorFormat, int frameRate, String mimeType) {
        MediaFormat mediaFormat = createMediaFormat(camcorderProfile.videoFrameWidth, camcorderProfile.videoFrameHeight, camcorderProfile.videoBitRate, camcorderProfile.videoFrameRate, colorFormat, frameRate, mimeType);
        return mediaFormat;
    }

    public static MediaFormat createMediaFormat(int videoFrameWidth, int videoFrameHeight, int videoBitRate,int videoFrameRate, int colorFormat, int frameRate, String mimeType) {
        MediaFormat mediaFormat = MediaFormat.createVideoFormat(mimeType, videoFrameWidth, videoFrameHeight);
        mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, videoBitRate);
        mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, videoFrameRate);
        mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, colorFormat);
        mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, frameRate);
        return mediaFormat;
    }
}