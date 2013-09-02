package com.ford.syncV4.android.activity.mobilenav;

import java.io.InputStream;

/**
 * Created by Andrew Batutin on 9/2/13.
 */
public class MockVideoDataSource implements Runnable {

    private InputStream inputStream;
    private VideoDataListener dataListener;
    private Thread thread;

    public MockVideoDataSource(InputStream inputStream, VideoDataListener dataListener) throws IllegalArgumentException {
        if (inputStream == null) {
            throw new IllegalArgumentException("input stream should not be null");
        }
        if (dataListener == null) {
            throw new IllegalArgumentException("listener should not be null");
        }
        this.inputStream = inputStream;
        this.dataListener = dataListener;
        thread = new Thread();
    }

    public InputStream getInputStream() {
        return inputStream;
    }

    public VideoDataListener getDataListener() {
        return dataListener;
    }

    @Override
    public void run() {

    }

    public void start() {
        if (thread.getState().equals(Thread.State.NEW)) {
            thread.start();
            dataListener.onStreamingStart();
        }
    }

    public Thread getThread() {
        return thread;
    }
}
