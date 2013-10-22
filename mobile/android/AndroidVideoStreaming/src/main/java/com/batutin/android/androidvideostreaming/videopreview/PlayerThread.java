package com.batutin.android.androidvideostreaming.videopreview;

import android.view.Surface;

import com.batutin.android.androidvideostreaming.media.VideoAvcCoder;
import com.batutin.android.androidvideostreaming.media.VideoAvcCoderDataStreamListener;
import com.batutin.android.androidvideostreaming.utils.ALog;

import java.io.PipedInputStream;

/**
 * Created by Andrew Batutin on 8/27/13.
 */
public class PlayerThread extends VideoPreviewThread {

    private VideoAvcCoder videoAvcCoder;
    private VideoAvcCoderDataStreamListener dataStreamListener;

    public PlayerThread(VideoAvcCoderDataStreamListener dataStreamListener, Surface surface, PipedInputStream pipedReader) {
        super();
        this.dataStreamListener = dataStreamListener;
        videoAvcCoder = VideoAvcCoder.createLowQualityVideoAvcCoder(surface, pipedReader);
        videoAvcCoder.setStreamListener(dataStreamListener);
        videoAvcCoder.start();
    }

    public VideoAvcCoderDataStreamListener getDataStreamListener() {
        return dataStreamListener;
    }

    public VideoAvcCoder getVideoAvcCoder() {
        return videoAvcCoder;
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


