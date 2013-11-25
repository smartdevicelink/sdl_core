package com.batutin.android.androidvideostreaming.videopreview;

import com.batutin.android.androidvideostreaming.utils.ALog;

/**
 * Created by Andrew Batutin on 8/27/13.
 */
public abstract class VideoPreviewThread extends Thread {

    protected VideoPreviewThread() {
        this.setUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
            @Override
            public void uncaughtException(Thread thread, Throwable ex) {
                defaultExceptionHandler(thread, ex);
            }
        });
    }

    protected abstract void defaultExceptionHandler(Thread paramThread, Throwable paramThrowable);

    protected void logUncaughtException(Thread paramThread, Throwable paramThrowable) {
        String logMessage = String.format("Thread %d Message %s", paramThread.getId(), paramThrowable.getMessage());
        ALog.e(logMessage);
    }
}
