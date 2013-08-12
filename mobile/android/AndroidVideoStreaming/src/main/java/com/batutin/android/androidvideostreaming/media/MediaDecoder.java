package com.batutin.android.androidvideostreaming.media;

import android.media.MediaCodec;
import android.media.MediaFormat;
import android.view.Surface;

import com.batutin.android.androidvideostreaming.activity.ALog;

/**
 * Created by Andrew Batutin on 8/12/13.
 */
public class MediaDecoder extends AbstractMediaCoder {


    public MediaCodec getDecoder() {
        return codec;
    }

    public MediaDecoder() {
        codec = createMediaDecoder();
    }


    private MediaCodec createMediaDecoder() {
        ALog.d("Start decoder creation");
        MediaCodec codec = createMediaCodec();
        ALog.d("End decoder creation");
        return codec;
    }

    @Override
    protected MediaCodec createMediaCodec() {
        MediaCodec codec = MediaCodec.createDecoderByType(MIME_TYPE);
        return codec;
    }

    public void configureMediaDecoder(MediaFormat format, Surface surface) {
        ALog.d("Start Decoder configure");
        mediaFormat = format;
        codec.configure(getMediaFormat(), surface, null, 0);
        isConfigured = true;
        ALog.d("End Decoder configure");
    }

    @Override
    public void start() throws IllegalStateException {
        if (isRunning == false) {
            ALog.d("Decoder is going to start");
        } else {
            ALog.d("Decoder is already started");
        }
        super.start();
        if (isRunning == true) {
            ALog.d("Decoder is started");
        } else {
            ALog.d("Decoder is stopped");
        }
    }

    @Override
    public void stop() throws IllegalStateException {
        if (isRunning == true) {
            ALog.d("Decoder is going to stop");
        } else {
            ALog.d("Decoder is already stopped");
        }
        super.stop();
        if (isRunning == false) {
            ALog.d("Decoder is stopped");
        } else {
            ALog.d("Decoder is running");
        }
    }
}
