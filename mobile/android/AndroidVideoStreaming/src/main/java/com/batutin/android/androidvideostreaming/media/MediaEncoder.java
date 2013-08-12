package com.batutin.android.androidvideostreaming.media;

import android.media.MediaCodec;
import android.media.MediaFormat;

import com.batutin.android.androidvideostreaming.activity.ALog;

/**
 * Created by Andrew Batutin on 8/9/13.
 */
public class MediaEncoder extends AbstractMediaCoder implements MediaCoderState {

    public MediaEncoder() {
        codec = createMediaEncoder();
    }

    public MediaCodec getEncoder() {
        return codec;
    }

    private MediaCodec createMediaEncoder() {
        ALog.d("Start codec creation");
        MediaCodec codec = createMediaCodec();
        ALog.d("End codec creation");
        return codec;
    }

    @Override
    protected MediaCodec createMediaCodec() {
        MediaCodec codec = MediaCodec.createEncoderByType(MIME_TYPE);
        return codec;
    }

    @Override
    public void start() throws IllegalStateException {
        if (isRunning == false){
            ALog.d("Encoder is going to start");
        } else {
            ALog.d("Encoder is already started");
        }
        super.start();
        if (isRunning == true){
            ALog.d("Encoder is started");
        } else {
            ALog.d("Encoder is stopped");
        }
    }

    @Override
    public void stop() throws IllegalStateException {
        if (isRunning == true){
            ALog.d("Encoder is going to stop");
        } else {
            ALog.d("Encoder is already stopped");
        }
        super.stop();
        if (isRunning == false){
            ALog.d("Encoder is stopped");
        } else {
            ALog.d("Encoder is running");
        }
    }

    public void configureMediaEncoder(MediaFormat format) {
        ALog.d("Start encoder configure");
        mediaFormat = format;
        codec.configure(getMediaFormat(), null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
        isConfigured = true;
        ALog.d("End encoder configure");
    }
}
