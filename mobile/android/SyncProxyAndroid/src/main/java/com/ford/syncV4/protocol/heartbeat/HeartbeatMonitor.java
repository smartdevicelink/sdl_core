package com.ford.syncV4.protocol.heartbeat;

import android.os.Handler;
import android.os.Looper;

import com.ford.syncV4.session.Session;
import com.ford.syncV4.util.logger.Logger;

/**
 * Concrete heartbeat monitor that runs on a separate, created thread.
 *
 * Created by enikolsky on 2013-12-26.
 */
public class HeartbeatMonitor implements IHeartbeatMonitor {

    public static final String HEARTBEAT_TIMEOUT_MSG = "Heartbeat timeout";
    /**
     * Interval between heartbeat messages, in milliseconds.
     * NOTE: this value is not specified in the protocol, and thus must be
     * negotiated with the Sync.
     */
    public static final int HEARTBEAT_INTERVAL = 5000;
    public static final int HEARTBEAT_INTERVAL_MAX = Integer.MAX_VALUE;

    private static final String CLASS_NAME = HeartbeatMonitor.class.getSimpleName();

    private final Object HeartbeatThreadHandler_Lock = new Object();
    private final Object Listener_Lock = new Object();

    private static int heartBeatInterval = HEARTBEAT_INTERVAL;
    private static boolean heartBeatAck = true;

    private IHeartbeatMonitorListener listener;
    private volatile boolean ackReceived;
    private volatile boolean heartbeatReceived;
    private Thread heartbeatThread;
    private Looper heartbeatThreadLooper;
    private Handler heartbeatThreadHandler;

    /**
     * Identifier of the Session to which this particular HB belongs
     */
    private byte mSessionId = Session.DEFAULT_SESSION_ID;

    public HeartbeatMonitor(byte sessionId) {
        this.mSessionId = sessionId;
    }

    private Runnable heartbeatTimeoutRunnable = new Runnable() {

        @Override
        public void run() {
            synchronized (Listener_Lock) {
                Logger.d(CLASS_NAME + " run():" + heartbeatReceived);
                if (heartbeatReceived) {
                    Logger.d(CLASS_NAME,
                            "Heartbeat has been received, sending and scheduling heartbeat");
                    if (listener != null) {
                        listener.sendHeartbeatACK(HeartbeatMonitor.this);
                    } else {
                        Logger.w(CLASS_NAME, "Listener is not set, scheduling heartbeat anyway");
                    }
                    heartbeatReceived = false;
                } else {
                    Logger.d(CLASS_NAME + " Heartbeat has not been received");
                    if (listener != null) {
                        listener.heartbeatTimedOut(HeartbeatMonitor.this);
                    }
                    // TODO stop?
                }
            }
        }

        private void rescheduleHeartbeat() {
            synchronized (HeartbeatThreadHandler_Lock) {
                if (heartbeatThreadHandler != null) {
                    if (!Thread.interrupted()) {
                        Logger.d(CLASS_NAME + " Rescheduling run()");
                        if (!heartbeatThreadHandler.postDelayed(this,
                                heartBeatInterval)) {
                            Logger.e(CLASS_NAME + " Couldn't reschedule run()");
                        }
                    } else {
                        Logger.i(CLASS_NAME,
                                " Thread is interrupted; not scheduling heartbeat");
                    }
                } else {
                    Logger.e(CLASS_NAME,
                            "Strange, Thread's handler is not set; not scheduling heartbeat");
                    HeartbeatMonitor.this.stop();
                }
            }
        }
    };

    private Runnable heartbeatAckTimeoutRunnable = new Runnable() {

        @Override
        public void run() {
            synchronized (Listener_Lock) {
                if (ackReceived) {
                    Logger.d(CLASS_NAME, " ACK has been received, sending and scheduling heartbeat");
                    if (listener != null) {
                        listener.sendHeartbeat(HeartbeatMonitor.this);
                    } else {
                        Logger.w(CLASS_NAME, " Delegate is not set, scheduling heartbeat anyway");
                    }
                    ackReceived = false;
                } else {
                    Logger.d(CLASS_NAME + " ACK has not been received");
                    if (listener != null) {
                        listener.heartbeatTimedOut(HeartbeatMonitor.this);
                    }
                    stop();
                }
            }
            rescheduleHeartbeat();
        }

        private void rescheduleHeartbeat() {
            synchronized (HeartbeatThreadHandler_Lock) {
                if (heartbeatThreadHandler != null) {
                    if (!Thread.interrupted()) {
                        Logger.d(CLASS_NAME + " Rescheduling run()");
                        if (!heartbeatThreadHandler.postDelayed(this, heartBeatInterval)) {
                            Logger.e(CLASS_NAME + " Couldn't reschedule run()");
                        }
                    } else {
                        Logger.i(CLASS_NAME, " The thread is interrupted; not scheduling heartbeat");
                    }
                } else {
                    Logger.e(CLASS_NAME,
                            " Strange, Thread's handler is not set; not scheduling heartbeat");
                    HeartbeatMonitor.this.stop();
                }
            }
        }
    };

    @Override
    public void start() {
        Logger.d(CLASS_NAME + " Start");
        synchronized (HeartbeatThreadHandler_Lock) {
            if (heartbeatThread == null) {
                heartbeatThread = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        if (!Thread.interrupted()) {
                            Looper.prepare();
                            heartbeatThreadLooper = Looper.myLooper();

                            heartbeatThreadHandler = new Handler();
                            Logger.d(CLASS_NAME + " scheduling run()");
                            ackReceived = true;
                            heartbeatReceived = true;
                            if (!heartbeatThreadHandler.postDelayed(
                                    heartbeatAckTimeoutRunnable, heartBeatInterval)) {
                                Logger.e(CLASS_NAME + " Couldn't schedule run()");
                            }

                            Logger.d(CLASS_NAME + " Starting looper");
                            Looper.loop();
                            Logger.d(CLASS_NAME + " Looper stopped, exiting thread");
                        } else {
                            Logger.i(CLASS_NAME,
                                    " Thread is run, but already interrupted");
                        }
                    }
                }, "HeartbeatThread");
                heartbeatThread.setPriority(Thread.MAX_PRIORITY);
                heartbeatThread.start();
            } else {
                Logger.d(CLASS_NAME + " Thread is already started; doing nothing");
            }
        }
    }

    @Override
    public void stop() {
        Logger.d(CLASS_NAME + " Stop");
        synchronized (HeartbeatThreadHandler_Lock) {
            if (heartbeatThread != null) {
                heartbeatThread.interrupt();
                heartbeatThread = null;

                if (heartbeatThreadHandler != null) {
                    heartbeatThreadHandler.removeCallbacks(heartbeatAckTimeoutRunnable);
                    heartbeatThreadHandler.removeCallbacks(heartbeatTimeoutRunnable);
                    heartbeatThreadHandler = null;
                } else {
                    Logger.e(CLASS_NAME + " Handler is null");
                }

                if (heartbeatThreadLooper != null) {
                    heartbeatThreadLooper.quit();
                    heartbeatThreadLooper = null;
                } else {
                    Logger.e(CLASS_NAME + " Looper is null");
                }
            } else {
                Logger.d(CLASS_NAME + " is not started");
                // just in case
                heartbeatThreadHandler = null;
                heartbeatThreadLooper = null;
            }
        }
    }

    @Override
    public int getInterval() {
        return heartBeatInterval;
    }

    @Override
    public void setInterval(int value) {
        heartBeatInterval = value;
    }

    @Override
    public boolean isHeartBeatAck() {
        return heartBeatAck;
    }

    @Override
    public void setHeartBeatAck(boolean value) {
        heartBeatAck = value;
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
    public void notifyTransportOutputActivity() {
        if (heartbeatThreadHandler == null) {
            return;
        }
        synchronized (HeartbeatThreadHandler_Lock) {
            if (heartbeatThreadHandler == null) {
                return;
            }
            heartbeatThreadHandler.removeCallbacks(heartbeatAckTimeoutRunnable);
            if (!heartbeatThreadHandler.postDelayed(heartbeatAckTimeoutRunnable, heartBeatInterval)) {
                Logger.e(CLASS_NAME + " Couldn't reschedule run()");
            }
        }
    }

    @Override
    public void notifyTransportInputActivity() {
        /*synchronized (HeartbeatThreadHandler_Lock) {
            if (heartbeatThreadHandler != null) {
                heartbeatThreadHandler.removeCallbacks(
                        heartbeatTimeoutRunnable);
                if (!heartbeatThreadHandler.postDelayed(
                        heartbeatTimeoutRunnable, heartBeatInterval)) {
                    Logger.e(CLASS_NAME + " Couldn't reschedule run()");
                }
            }
        }*/
    }

    @Override
    public synchronized void heartbeatACKReceived() {
        synchronized (Listener_Lock) {
            ackReceived = true;
        }
    }

    @Override
    public void heartbeatReceived() {
        if (heartbeatThreadHandler == null) {
            return;
        }
        synchronized (Listener_Lock) {
            if (heartBeatAck) {
                heartbeatReceived = true;
                if (!heartbeatThreadHandler.post(heartbeatTimeoutRunnable)) {
                    Logger.e(CLASS_NAME + " Couldn't schedule run()");
                }
            }
        }
    }

    @Override
    public byte getSessionId() {
        return mSessionId;
    }
}