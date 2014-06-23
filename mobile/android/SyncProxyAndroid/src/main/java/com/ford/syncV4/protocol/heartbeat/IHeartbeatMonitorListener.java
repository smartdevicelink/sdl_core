package com.ford.syncV4.protocol.heartbeat;

/**
 * Listener (aka delegate) that actually knows how to work with the outside
 * world.
 */
public interface IHeartbeatMonitorListener {
    /**
     * The listener should create and send a heartbeat message.
     *
     * @param monitor the caller
     */
    public void sendHeartbeat(IHeartbeatMonitor monitor);

    /**
     * Notifies the listener that the heartbeat timed out.
     *
     * @param monitor the caller
     */
    public void heartbeatTimedOut(IHeartbeatMonitor monitor);

    public void sendHeartbeatACK(IHeartbeatMonitor heartbeatMonitor);
}
