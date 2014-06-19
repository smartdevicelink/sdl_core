package com.ford.syncV4.protocol.heartbeat;

import com.ford.syncV4.util.logger.Logger;

import java.util.ArrayList;
import java.util.List;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 17.06.14
 * Time: 12:56
 */
public class HeartbeatMonitorsManager {

    private static final String CLASS_NAME = HeartbeatMonitorsManager.class.getSimpleName();

    private volatile List<IHeartbeatMonitor> heartbeatMonitors = new ArrayList<IHeartbeatMonitor>();

    public HeartbeatMonitorsManager() {
        Logger.d(CLASS_NAME + " Constructor");
    }

    public synchronized void addMonitor(IHeartbeatMonitor monitor) {
        addMonitor(monitor, "private");
    }

    public synchronized void addMonitor(IHeartbeatMonitor monitor, String source) {
        heartbeatMonitors.add(monitor);
        Logger.d(CLASS_NAME + " add monitor:" + monitor + ", size:" + heartbeatMonitors.size());
    }

    public synchronized void addMonitor(byte sessionId, int heartBeatInterval, boolean heartBeatAck) {
        if (hasMonitor(sessionId)) {
            return;
        }
        IHeartbeatMonitor monitor = HeartbeatBuilder.buildHeartbeatMonitor(sessionId,
                heartBeatInterval, heartBeatAck);
        addMonitor(monitor, HeartbeatMonitorsManager.class.getSimpleName());
    }

    public synchronized boolean hasMonitor(byte sessionId) {
        for (IHeartbeatMonitor monitor : heartbeatMonitors) {
            if (monitor == null) {
                continue;
            }
            if (monitor.getSessionId() == sessionId) {
                Logger.d(CLASS_NAME + " has monitor for sesId:" + sessionId);
                return true;
            }
        }
        Logger.d(CLASS_NAME + " has no monitor for sesId:" + sessionId);
        return false;
    }

    public synchronized void startMonitor(byte sessionId, IHeartbeatMonitorListener listener) {
        Logger.d(CLASS_NAME + " start monitor for sesId:" + sessionId);
        for (IHeartbeatMonitor monitor : heartbeatMonitors) {
            if (monitor == null) {
                continue;
            }
            if (monitor.getSessionId() == sessionId) {
                monitor.setListener(listener);
                monitor.start();
                Logger.d(CLASS_NAME + " monitor started, sesId:" + sessionId);
                return;
            }
        }
    }

    public synchronized void stopMonitor(byte sessionId) {
        Logger.d(CLASS_NAME + " stop monitor for sesId:" + sessionId);
        for (IHeartbeatMonitor monitor : heartbeatMonitors) {
            if (monitor == null) {
                continue;
            }
            if (monitor.getSessionId() == sessionId) {
                monitor.setListener(null);
                monitor.stop();
                Logger.d(CLASS_NAME + " monitor stopped, sesId:" + sessionId);
                return;
            }
        }
    }

    public void stopAllMonitors() {
        Logger.d(CLASS_NAME + " stop all monitors");
        for (IHeartbeatMonitor monitor : heartbeatMonitors) {
            if (monitor == null) {
                continue;
            }
            monitor.stop();
            Logger.d(CLASS_NAME + " monitor stopped, sesId:" + monitor.getSessionId());
        }
    }

    public synchronized void heartbeatACKReceived(byte sessionId) {
        Logger.d(CLASS_NAME + " heartbeatACKReceived for sesId:" + sessionId);
        for (IHeartbeatMonitor monitor : heartbeatMonitors) {
            if (monitor == null) {
                continue;
            }
            if (monitor.getSessionId() == sessionId) {
                monitor.heartbeatACKReceived();
                Logger.d(CLASS_NAME + " heartbeatACKReceived done sesId:" + sessionId);
                return;
            }
        }
    }

    public synchronized void heartbeatReceived(byte sessionId) {
        Logger.d(CLASS_NAME + " heartbeatReceived for sesId:" + sessionId);
        for (IHeartbeatMonitor monitor : heartbeatMonitors) {
            if (monitor == null) {
                continue;
            }
            if (monitor.getSessionId() == sessionId) {
                monitor.heartbeatReceived();
                Logger.d(CLASS_NAME + " heartbeatReceived done sesId:" + sessionId);
                return;
            }
        }
    }

    public synchronized void notifyTransportOutputActivity(byte sessionId) {
        Logger.d(CLASS_NAME + " notifyTransportOutputActivity for sesId:" + sessionId);
        for (IHeartbeatMonitor monitor : heartbeatMonitors) {
            if (monitor == null) {
                continue;
            }
            if (monitor.getSessionId() == sessionId) {
                monitor.notifyTransportOutputActivity();
                Logger.d(CLASS_NAME + " notifyTransportOutputActivity done sesId:" + sessionId);
                return;
            }
        }
    }

    public void notifyTransportInputActivity(byte sessionId) {
        /*Logger.d(CLASS_NAME + " notifyTransportInputActivity for sesId:" + sessionId);
        for (IHeartbeatMonitor monitor : heartbeatMonitors) {
            if (monitor == null) {
                continue;
            }
            if (monitor.getSessionId() == sessionId) {
                monitor.notifyTransportInputActivity();
                Logger.d(CLASS_NAME + " notifyTransportInputActivity done sesId:" + sessionId);
                return;
            }
        }*/
    }

    public void dispose() {
        Logger.d(CLASS_NAME + " dispose");
        for (IHeartbeatMonitor monitor : heartbeatMonitors) {
            if (monitor == null) {
                continue;
            }
            monitor.setListener(null);
            monitor.stop();
            Logger.d(CLASS_NAME + " dispose done sesId:" + monitor.getSessionId());
            monitor = null;
        }
        heartbeatMonitors.clear();
        Logger.d(CLASS_NAME + " dispose complete");
    }
}