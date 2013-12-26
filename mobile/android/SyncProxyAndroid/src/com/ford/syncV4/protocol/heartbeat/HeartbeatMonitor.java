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

    @Override
    public void start() {
        final Runnable heartbeatTimeout = new Runnable() {
            @Override
            public void run() {
                Log.d(TAG, "run()");
                if (delegate != null) {
                    delegate.sendHeartbeat(HeartbeatMonitor.this);
                }

                new Handler().postDelayed(this, interval);
            }
        };

        Thread heartbeatThread = new Thread(new Runnable() {
            @Override
            public void run() {
                Looper.prepare();

                final Handler handler = new Handler();
                Log.d(TAG, "scheduling run()");
                if (!handler.postDelayed(heartbeatTimeout, interval)) {
                    Log.e(TAG, "Couldn't schedule run()");
                }

                Looper.loop();
            }
        }, "HeartbeatThread");
        heartbeatThread.start();
    }

    @Override
    public void stop() {

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
