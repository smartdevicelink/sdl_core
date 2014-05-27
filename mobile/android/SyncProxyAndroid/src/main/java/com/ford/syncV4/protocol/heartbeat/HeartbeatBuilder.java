package com.ford.syncV4.protocol.heartbeat;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 26.05.14
 * Time: 14:45
 */
public class HeartbeatBuilder {

    public static IHeartbeatMonitor buildHeartbeatMonitor(byte sessionId, int heartBeatInterval,
                                                          boolean heartBeatAck) {
        final HeartbeatMonitor heartbeatMonitor = new HeartbeatMonitor(sessionId);
        heartbeatMonitor.setInterval(heartBeatInterval);
        heartbeatMonitor.isSendHeartbeatAck(heartBeatAck);
        return heartbeatMonitor;
    }
}