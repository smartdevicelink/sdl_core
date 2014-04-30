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
     * Returns the listener.
     *
     * @return the listener
     */
    public IHeartbeatMonitorListener getListener();

    /**
     * Sets the heartbeat's listener.
     *
     * @param listener the new listener
     */
    public void setListener(IHeartbeatMonitorListener listener);

    /**
     * Notifies the monitor about sent/received messages.
     */
    public void notifyTransportOutputActivity();

    void notifyTransportInputActivity();

    /**
     * Notifies the monitor about a received heartbeat ACK message.
     */
    public void heartbeatACKReceived();

    public void heartbeatReceived();
}
