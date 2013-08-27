package com.batutin.android.androidvideostreaming.videopreview;

import android.view.Surface;

import com.batutin.android.androidvideostreaming.activity.DecodeActivity;
import com.batutin.android.androidvideostreaming.media.VideoAvcCoder;
import com.batutin.android.androidvideostreaming.utils.ALog;

import java.io.PipedInputStream;

/**
 * Created by Andrew Batutin on 8/27/13.
 */
public class PlayerThread extends VideoPreviewThread {

    public VideoAvcCoder videoAvcCoder;
    private DecodeActivity decodeActivity;

    public PlayerThread(DecodeActivity decodeActivity, Surface surface, PipedInputStream pipedReader) {
        super();
        this.decodeActivity = decodeActivity;
        videoAvcCoder = VideoAvcCoder.createLowQualityVideoAvcCoder(surface, pipedReader);
        videoAvcCoder.setStreamListener(decodeActivity);
        videoAvcCoder.start();
    }

    @Override
    public String toString() {
        String message = videoAvcCoder.toString();
        return super.toString() + message;
    }

    @Override
    public void run() {
        videoAvcCoder.doEncodeDecodeVideoFromBuffer();
    }

    @Override
    protected void defaultExceptionHandler(Thread paramThread, Throwable paramThrowable) {
        logUncaughtException(paramThread, paramThrowable);
        try {
            videoAvcCoder.stop();
        } catch (IllegalStateException e) {
            ALog.e(e.getMessage());
        }
        interrupt();
    }
}


