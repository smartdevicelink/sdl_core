package com.batutin.android.androidvideostreaming.videopreview;

import com.batutin.android.androidvideostreaming.activity.DecodeActivity;
import com.batutin.android.androidvideostreaming.utils.ALog;

import java.io.OutputStream;

/**
 * Created by Andrew Batutin on 8/27/13.
 */
public class BitmapGeneratorThread extends VideoPreviewThread {

    private final OutputStream pipedOutputStream;
    private DecodeActivity decodeActivity;
    private boolean stop = false;

    public BitmapGeneratorThread(DecodeActivity decodeActivity, OutputStream pipedOutputStream) {
        super();
        this.decodeActivity = decodeActivity;
        this.pipedOutputStream = pipedOutputStream;
    }

    public synchronized void shouldStop() {
        this.stop = true;
    }

    public synchronized OutputStream getPipedOutputStream() {
        return pipedOutputStream;
    }

    @Override
    public void run() {
        try {
            while (!stop) {
                pipedOutputStream.write(decodeActivity.createTestByteArray());
                pipedOutputStream.flush();
            }
            pipedOutputStream.close();
            decodeActivity.getPlayer().videoAvcCoder.shouldStop();
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
