package com.ford.syncV4.protocol.heartbeat;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;

/**
 * Concrete heartbeat monitor that runs on a separate, created thread.
 *
 * Created by enikolsky on 2013-12-26.
 */
public class HeartbeatMonitor implements IHeartbeatMonitor {
    private static final String TAG = HeartbeatMonitor.class.getSimpleName();
    private int interval;
    private IHeartbeatMonitorDelegate delegate;
    private Thread heartbeatThread;
    private Looper heartbeatThreadLooper;
    private Handler heartbeatThreadHandler;
    private Runnable heartbeatTimeoutRunnable = new Runnable() {
        @Override
        public void run() {
            Log.d(TAG, "run()");
            if (delegate != null) {
                delegate.sendHeartbeat(HeartbeatMonitor.this);
            } else {
                Log.w(TAG, "Delegate is not set, scheduling heartbeat anyway");
            }

            if (heartbeatThreadHandler != null) {
                if (!Thread.interrupted()) {
                    heartbeatThreadHandler.postDelayed(this, interval);
                } else {
                    Log.i(TAG,
                            "The thread is interrupted; not scheduling heartbeat");
                }
            } else {
                Log.e(TAG,
                        "Strange, HeartbeatThread's handler is not set; not scheduling heartbeat");
                HeartbeatMonitor.this.stop();
            }
        }
    };

    @Override
    public void start() {
        heartbeatThread = new Thread(new Runnable() {
            @Override
            public void run() {
                if (!Thread.interrupted()) {
                    Looper.prepare();
                    heartbeatThreadLooper = Looper.myLooper();

                    heartbeatThreadHandler = new Handler();
                    Log.d(TAG, "scheduling run()");
                    if (!heartbeatThreadHandler.postDelayed(
                            heartbeatTimeoutRunnable, interval)) {
                        Log.e(TAG, "Couldn't schedule run()");
                    }

                    Log.d(TAG, "Starting looper");
                    Looper.loop();
                    Log.d(TAG, "Looper stopped, exiting thread");
                } else {
                    Log.i(TAG,
                            "HeartbeatThread is run, but already interrupted");
                }
            }
        }, "HeartbeatThread");
        heartbeatThread.start();
    }

    @Override
    public void stop() {
        if (heartbeatThread != null) {
            heartbeatThread.interrupt();
            heartbeatThread = null;

            if (heartbeatThreadHandler != null) {
                heartbeatThreadHandler.removeCallbacks(
                        heartbeatTimeoutRunnable);
                heartbeatThreadHandler = null;
            } else {
                Log.e(TAG, "HeartbeatThread's handler is null");
            }

            if (heartbeatThreadLooper != null) {
                heartbeatThreadLooper.quit();
                heartbeatThreadLooper = null;
            } else {
                Log.e(TAG, "HeartbeatThread's looper is null");
            }
        } else {
            Log.d(TAG, "HeartbeatThread is not started");
            // just in case
            heartbeatThreadHandler = null;
            heartbeatThreadLooper = null;
        }
    }

    @Override
    public int getInterval() {
        return interval;
    }

    @Override
    public void setInterval(int interval) {
        this.interval = interval;
    }

    @Override
    public IHeartbeatMonitorDelegate getDelegate() {
        return delegate;
    }

    @Override
    public void setDelegate(IHeartbeatMonitorDelegate delegate) {
        this.delegate = delegate;
    }

    @Override
    public void notifyTransportActivity() {

    }

    @Override
    public void heartbeatACKReceived() {

    }
}
