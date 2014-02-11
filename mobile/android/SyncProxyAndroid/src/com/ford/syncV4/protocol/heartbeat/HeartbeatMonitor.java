package com.ford.syncV4.protocol.heartbeat;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;

/**
 * Concrete heartbeat monitor that runs on a separate, created thread.
 * <p/>
 * Created by enikolsky on 2013-12-26.
 */
public class HeartbeatMonitor implements IHeartbeatMonitor {
    private static final String TAG = HeartbeatMonitor.class.getSimpleName();
    private final Object HeartbeatThreadHandler_Lock = new Object();
    private final Object Listener_Lock = new Object();
    //
    private int interval;
    private IHeartbeatMonitorListener listener;
    private boolean ackReceived;
    private Thread heartbeatThread;
    private Looper heartbeatThreadLooper;
    private Handler heartbeatThreadHandler;
    private Runnable heartbeatTimeoutRunnable = new Runnable() {
        @Override
        public void run() {
            synchronized (Listener_Lock) {
                Log.d(TAG, "run()");

                if (ackReceived) {
                    Log.d(TAG,
                            "ACK has been received, sending and scheduling heartbeat");
                    if (listener != null) {
                        listener.sendHeartbeat(HeartbeatMonitor.this);
                    } else {
                        Log.w(TAG,
                                "Delegate is not set, scheduling heartbeat anyway");
                    }
                    ackReceived = false;
                } else {
                    Log.d(TAG, "ACK has not been received");
                    if (listener != null) {
                        listener.heartbeatTimedOut(HeartbeatMonitor.this);
                    }
                    // TODO stop?
                }
            }
            synchronized (HeartbeatThreadHandler_Lock) {
                if (heartbeatThreadHandler != null) {
                    if (!Thread.interrupted()) {
                        Log.d(TAG, "Rescheduling run()");
                        if (!heartbeatThreadHandler.postDelayed(this,
                                interval)) {
                            Log.e(TAG, "Couldn't reschedule run()");
                        }
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
        }
    };

    @Override
    public void start() {
        synchronized (HeartbeatThreadHandler_Lock) {
            if (heartbeatThread == null) {
                heartbeatThread = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        if (!Thread.interrupted()) {
                            Looper.prepare();
                            heartbeatThreadLooper = Looper.myLooper();

                            heartbeatThreadHandler = new Handler();
                            Log.d(TAG, "scheduling run()");
                            ackReceived = true;
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
            } else {
                Log.d(TAG, "HeartbeatThread is already started; doing nothing");
            }
        }
    }

    @Override
    public void stop() {
        synchronized (HeartbeatThreadHandler_Lock) {
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
    public IHeartbeatMonitorListener getListener() {
        return listener;
    }

    @Override
    public void setListener(IHeartbeatMonitorListener listener) {
        this.listener = listener;
    }

    @Override
    public void notifyTransportActivity() {
        synchronized (HeartbeatThreadHandler_Lock) {
            if (heartbeatThreadHandler != null) {
                heartbeatThreadHandler.removeCallbacks(
                        heartbeatTimeoutRunnable);
                if (!heartbeatThreadHandler.postDelayed(
                        heartbeatTimeoutRunnable, interval)) {
                    Log.e(TAG, "Couldn't reschedule run()");
                }
            }
        }
    }

    @Override
    public void heartbeatACKReceived() {
        synchronized (HeartbeatThreadHandler_Lock) {
            Log.d(TAG, "ACK received");
            ackReceived = true;
        }
    }
}
