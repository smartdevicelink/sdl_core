package com.batutin.android.androidvideostreaming.videopreview;

import com.batutin.android.androidvideostreaming.utils.ALog;

import java.io.OutputStream;

/**
 * Created by Andrew Batutin on 8/27/13.
 */
public class BitmapGeneratorThread extends VideoPreviewThread {

    private PlayerThreadState threadState;
    private FrameDataSource dataSource;
    private OutputStream outputStream;

    private boolean stop = false;

    public BitmapGeneratorThread(OutputStream outputStream, PlayerThreadState threadState, FrameDataSource dataSource) {
        super();
        this.threadState = threadState;
        this.dataSource = dataSource;
        this.outputStream = outputStream;
    }

    public synchronized void shouldStop() {
        this.stop = true;
    }

    public synchronized OutputStream getOutputStream() {
        return outputStream;
    }

    @Override
    public void run() {
        try {
            while (!stop) {
                outputStream.write(dataSource.createVideoFrame());
                outputStream.flush();
            }
            outputStream.close();
            threadState.threadShouldStop(this);
        } catch (Exception e) {
            ALog.e(e.getMessage());
        }
    }

    @Override
    protected void defaultExceptionHandler(Thread paramThread, Throwable paramThrowable) {
        logUncaughtException(paramThread, paramThrowable);
        try {
            shouldStop();
        } catch (IllegalStateException e) {
            ALog.e(e.getMessage());
        }
        interrupt();
    }
}
