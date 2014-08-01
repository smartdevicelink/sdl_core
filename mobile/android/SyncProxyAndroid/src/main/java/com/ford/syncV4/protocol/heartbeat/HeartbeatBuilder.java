package com.ford.syncV4.protocol.heartbeat;

import com.ford.syncV4.util.logger.Logger;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 26.05.14
 * Time: 14:45
 */
public class HeartbeatBuilder {

    private static final String CLASS_NAME = HeartbeatBuilder.class.getSimpleName();

    public static IHeartbeatMonitor buildHeartbeatMonitor(byte sessionId, int heartBeatInterval,
                                                          boolean heartBeatAck) {
        final HeartbeatMonitor heartbeatMonitor = new HeartbeatMonitor(sessionId);
        heartbeatMonitor.setInterval(heartBeatInterval);
        heartbeatMonitor.setHeartBeatAck(heartBeatAck);
        Logger.d(CLASS_NAME + " HBM has been created, sesId:" + sessionId +
                ", interval:" + heartBeatInterval +
                ", ack:" + heartBeatAck);
        return heartbeatMonitor;
    }
}