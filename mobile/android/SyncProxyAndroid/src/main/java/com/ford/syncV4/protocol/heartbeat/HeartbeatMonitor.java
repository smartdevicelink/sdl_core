package com.ford.syncV4.protocol.heartbeat;

import android.os.Handler;
import android.os.Looper;

import com.ford.syncV4.util.logger.Logger;

/**
 * Concrete heartbeat monitor that runs on a separate, created thread.
 * <p/>
 * Created by enikolsky on 2013-12-26.
 */
public class HeartbeatMonitor implements IHeartbeatMonitor {
    private static final String CLASS_NAME = HeartbeatMonitor.class.getSimpleName();
    private final Object HeartbeatThreadHandler_Lock = new Object();
    private final Object Listener_Lock = new Object();
    //
    private int count = 0;
    private int interval;
    private boolean isHeartbeatAck = true;
    private IHeartbeatMonitorListener listener;
    private boolean ackReceived;
    private boolean heartbeatReceived;
    private Thread heartbeatThread;
    private Looper heartbeatThreadLooper;
    private Handler heartbeatThreadHandler;

    private Runnable heartbeatTimeoutRunnable = new Runnable() {
        @Override
        public void run() {
            synchronized (Listener_Lock) {
                Logger.d(CLASS_NAME + " run()");

                if (heartbeatReceived) {


                    Logger.d(CLASS_NAME,
                            "Heartbeat has been received, sending and scheduling heartbeat");
                    if (listener != null) {
                        listener.sendHeartbeatACK(HeartbeatMonitor.this);
                    } else {
                        Logger.w(CLASS_NAME,
                                "Delegate is not set, scheduling heartbeat anyway");
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
                                interval)) {
                            Logger.e(CLASS_NAME + " Couldn't reschedule run()");
                        }
                    } else {
                        Logger.i(CLASS_NAME,
                                "The thread is interrupted; not scheduling heartbeat");
                    }
                } else {
                    Logger.e(CLASS_NAME,
                            "Strange, HeartbeatThread's handler is not set; not scheduling heartbeat");
                    HeartbeatMonitor.this.stop();
                }
            }
        }
    };

    private Runnable heartbeatAckTimeoutRunnable = new Runnable() {
        @Override
        public void run() {
            synchronized (Listener_Lock) {
                Logger.d(CLASS_NAME + " run()");

                if (ackReceived) {
                    Logger.d(CLASS_NAME,
                            " ACK has been received, sending and scheduling heartbeat");
                    if (listener != null) {
                        listener.sendHeartbeat(HeartbeatMonitor.this);
                    } else {
                        Logger.w(CLASS_NAME,
                                " Delegate is not set, scheduling heartbeat anyway");
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
                        if (!heartbeatThreadHandler.postDelayed(this,
                                interval)) {
                            Logger.e(CLASS_NAME + " Couldn't reschedule run()");
                        }
                    } else {
                        Logger.i(CLASS_NAME,
                                " The thread is interrupted; not scheduling heartbeat");
                    }
                } else {
                    Logger.e(CLASS_NAME,
                            " Strange, HeartbeatThread's handler is not set; not scheduling heartbeat");
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
                            Logger.d(CLASS_NAME + " scheduling run()");
                            ackReceived = true;
                            heartbeatReceived = true;
                            if (!heartbeatThreadHandler.postDelayed(
                                    heartbeatAckTimeoutRunnable, interval)) {
                                Logger.e(CLASS_NAME + " Couldn't schedule run()");
                            }

                            Logger.d(CLASS_NAME + " Starting looper");
                            Looper.loop();
                            Logger.d(CLASS_NAME + " Looper stopped, exiting thread");
                        } else {
                            Logger.i(CLASS_NAME,
                                    " HeartbeatThread is run, but already interrupted");
                        }
                    }
                }, "HeartbeatThread");
                heartbeatThread.setPriority(Thread.MAX_PRIORITY);
                heartbeatThread.start();
            } else {
                Logger.d(CLASS_NAME + " HeartbeatThread is already started; doing nothing");
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
                            heartbeatAckTimeoutRunnable);
                    heartbeatThreadHandler.removeCallbacks(
                            heartbeatTimeoutRunnable);
                    heartbeatThreadHandler = null;
                } else {
                    Logger.e(CLASS_NAME + " HeartbeatThread's handler is null");
                }

                if (heartbeatThreadLooper != null) {
                    heartbeatThreadLooper.quit();
                    heartbeatThreadLooper = null;
                } else {
                    Logger.e(CLASS_NAME + " HeartbeatThread's looper is null");
                }
            } else {
                Logger.d(CLASS_NAME + " HeartbeatThread is not started");
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
    public void notifyTransportOutputActivity() {
        synchronized (HeartbeatThreadHandler_Lock) {
            if (heartbeatThreadHandler != null) {
                heartbeatThreadHandler.removeCallbacks(
                        heartbeatAckTimeoutRunnable);
                if (!heartbeatThreadHandler.postDelayed(
                        heartbeatAckTimeoutRunnable, interval)) {
                    Logger.e(CLASS_NAME + " Couldn't reschedule run()");
                }
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
                        heartbeatTimeoutRunnable, interval)) {
                    Logger.e(CLASS_NAME + " Couldn't reschedule run()");
                }
            }
        }*/
    }

    @Override
    public void heartbeatACKReceived() {
        synchronized (Listener_Lock) {
            Logger.d(CLASS_NAME + " ACK received");
            ackReceived = true;
        }
    }

    @Override
    public void heartbeatReceived() {
        Logger.d(CLASS_NAME + " Heartbeat received, isHeartbeatAck:" + isHeartbeatAck);
        synchronized (Listener_Lock) {
            if (isHeartbeatAck) {
                Logger.d(CLASS_NAME + " Heartbeat start do post");
                heartbeatReceived = true;
                count += 1;
                if (!heartbeatThreadHandler.post(heartbeatTimeoutRunnable)) {
                    Logger.e(CLASS_NAME + " Couldn't schedule run()");
                }
            }
        }
    }

    public void isSendHeartbeatAck(boolean heartBeatAck) {
        isHeartbeatAck = heartBeatAck;
    }
}
