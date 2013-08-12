package com.batutin.android.androidvideostreaming.media;

public class EncoderMediaFormatSetting {
    private final int videoFrameWidth;
    private final int videoFrameHeight;
    private final int videoBitRate;
    private final int videoFrameRate;
    private final int colorFormat;
    private final int frameRate;
    private final String mimeType;

    public EncoderMediaFormatSetting(int videoFrameWidth, int videoFrameHeight, int videoBitRate, int videoFrameRate, int colorFormat, int frameRate, String mimeType) {
        this.videoFrameWidth = videoFrameWidth;
        this.videoFrameHeight = videoFrameHeight;
        this.videoBitRate = videoBitRate;
        this.videoFrameRate = videoFrameRate;
        this.colorFormat = colorFormat;
        this.frameRate = frameRate;
        this.mimeType = mimeType;
    }

    public int getVideoFrameWidth() {
        return videoFrameWidth;
    }

    public int getVideoFrameHeight() {
        return videoFrameHeight;
    }

    public int getVideoBitRate() {
        return videoBitRate;
    }

    public int getVideoFrameRate() {
        return videoFrameRate;
    }

    public int getColorFormat() {
        return colorFormat;
    }

    public int getFrameRate() {
        return frameRate;
    }

    public String getMimeType() {
        return mimeType;
    }
}
