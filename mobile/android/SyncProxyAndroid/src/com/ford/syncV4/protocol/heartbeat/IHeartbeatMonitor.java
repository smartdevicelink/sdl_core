package com.ford.syncV4.protocol.heartbeat;

/**
 * Common interface for protocol heartbeat implementations.
 *
 * Created by enikolsky on 2013-12-25.
 */
public interface IHeartbeatMonitor {
    /**
     * Starts the monitor. If the monitor is already started, nothing happens.
     */
    public void start();

    /**
     * Stops the monitor. Does nothing if it is already stopped.
     */
    public void stop();

    /**
     * Returns the heartbeat messages interval.
     *
     * @return interval in milliseconds
     */
    public int getInterval();

    /**
     * Sets the interval for sending heartbeat messages if nothing is sent over
     * transport.
     *
     * @param interval interval in milliseconds (min/max values depend on
     *                 concrete implementations)
     */
    public void setInterval(int interval);

    /**
     * Returns the delegate.
     *
     * @return the delegate
     */
    public IHeartbeatMonitorDelegate getDelegate();

    /**
     * Sets the heartbeat's delegate.
     *
     * @param delegate the new delegate
     */
    public void setDelegate(IHeartbeatMonitorDelegate delegate);

    /**
     * Notifies the monitor about sent/received messages.
     */
    public void notifyTransportActivity();

    /**
     * Notifies the monitor about a received heartbeat ACK message.
     */
    public void heartbeatACKReceived();

    /**
     * Delegate that actually knows how to work with the outside world.
     */
    public static interface IHeartbeatMonitorDelegate {
        /**
         * The delegate should create and send a hearbeat message.
         *
         * @param monitor the caller
         */
        public void sendHeartbeat(IHeartbeatMonitor monitor);

        /**
         * Notifies the delegate that the heartbeat timed out.
         *
         * @param monitor the caller
         */
        public void heartbeatTimedOut(IHeartbeatMonitor monitor);
    }
}
