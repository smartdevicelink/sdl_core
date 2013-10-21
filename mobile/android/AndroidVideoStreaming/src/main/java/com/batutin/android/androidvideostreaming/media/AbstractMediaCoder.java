package com.batutin.android.androidvideostreaming.media;

import android.media.MediaCodec;
import android.media.MediaFormat;

/**
 * Created by Andrew Batutin on 8/12/13.
 */
public abstract class AbstractMediaCoder implements MediaCoderState {
    public static final String MIME_TYPE = "video/avc";
    protected boolean isConfigured = false;
    protected boolean isRunning = false;
    protected MediaFormat mediaFormat;
    protected MediaCodec codec;

    public boolean isRunning() {
        return isRunning;
    }

    public boolean isConfigured() {
        return isConfigured;
    }

    public MediaFormat getMediaFormat() {
        return mediaFormat;
    }

    @Override
    public void start() throws IllegalStateException {
        if (isRunning == false) {
            codec.start();
            isRunning = true;
        }
    }

    @Override
    public void stop() throws IllegalStateException {
        if (isRunning == true) {
            codec.stop();
            codec.release();
            isRunning = false;
        }
    }

    protected abstract MediaCodec createMediaCodec();
}
