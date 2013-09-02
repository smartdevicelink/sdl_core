package com.ford.syncV4.android.activity.mobilenav;

/**
 * Created by Andrew Batutin on 9/2/13.
 */
public class MockVideoDataSource implements Runnable {

    private static byte[] mockData = new byte[100];
    private VideoDataListener dataListener;
    private Thread thread;

    public MockVideoDataSource(VideoDataListener dataListener) throws IllegalArgumentException {
        if (dataListener == null) {
            throw new IllegalArgumentException("listener should not be null");
        }

        this.dataListener = dataListener;
        thread = new Thread(this);
    }

    public VideoDataListener getDataListener() {
        return dataListener;
    }

    public Thread getThread() {
        return thread;
    }

    @Override
    public void run() {
        while (!Thread.interrupted()) {
            dispatchDataToListener();
        }
    }

    public void dispatchDataToListener() {
        dataListener.videoFrameReady(createMockData());
    }

    byte[] createMockData() {
        return mockData;
    }

    public synchronized void start() {
        if (thread.getState().equals(Thread.State.NEW)) {
            thread.start();
            dataListener.onStreamingStart();
        }
    }

    public synchronized void stop() {
        thread.interrupt();
        dataListener.onStreamStop();
    }
}
