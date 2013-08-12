package com.batutin.android.androidvideostreaming.media;

import android.media.MediaCodec;
import android.media.MediaFormat;

import com.batutin.android.androidvideostreaming.activity.ALog;

/**
 * Created by Andrew Batutin on 8/9/13.
 */
public class MediaEncoder {

    public static final String MIME_TYPE = "video/avc";
    private MediaCodec encoder;
    private MediaFormat mediaFormat;
    private boolean isConfigured = false;
    private boolean isRunning = false;

    public MediaEncoder() {
        encoder = createVideoAvcEncoder();
    }

    public boolean isRunning() {
        return isRunning;
    }

    public boolean isConfigured() {
        return isConfigured;
    }

    public MediaFormat getMediaFormat() {
        return mediaFormat;
    }

    public MediaCodec getEncoder() {
        return encoder;
    }

    public void start() throws IllegalStateException {
        if (isRunning == false) {
            encoder.start();
            isRunning = true;
            ALog.d("Encoder started");
        } else {
            ALog.d("Encoder already have been started");
        }
    }

    public void stop() {
        if (isRunning == true) {
            encoder.stop();
            encoder.release();
            isRunning = false;
            ALog.d("Encoder stopped");
        } else {
            ALog.d("Encoder have not been started yet");
        }
    }

    public void configureMediaEncoder(MediaFormat format) {
        ALog.d("Start encoder configure");
        mediaFormat = format;
        encoder.configure(mediaFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
        isConfigured = true;
        ALog.d("End encoder configure");
    }

    private MediaCodec createVideoAvcEncoder() {
        ALog.d("Start encoder creation");
        MediaCodec codec = MediaCodec.createEncoderByType(MIME_TYPE);
        ALog.d("End encoder creation");
        return codec;
    }
}
