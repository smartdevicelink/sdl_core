package com.ford.syncV4.transport;

/**
 * Container of TCP transport specific configuration. 
 */
public class TCPTransportConfig extends BaseTransportConfig {
	
	/**
	 * Value of port to use in TCP connection.
	 */
	private final int mPort;
	
	/**
	 * Value of IP address to use in TCP connection.
	 */
	private final String mIpAddress;

    /**
     * Value of flag which is set to true if tcp connection must be automatically reestablished in case of disconnection
     */
    private final boolean mAutoReconnect;

    /**
	 * Constructor. Objects of this class must be created for known port and IP address value.
	 * 
	 * @param port Port for TCP connection.
	 * @param ipAddress IP address for TCP connection.
     * @param autoReconnect Flag which must be set to true if tcp connection must be automatically reestablished in
     *                      case of disconnection
	 */
    public TCPTransportConfig(int port, String ipAddress, boolean autoReconnect) {
		mPort = port;
		mIpAddress = ipAddress;
        mAutoReconnect = autoReconnect;
    }
	
	/**
	 * Gets value of Port.
	 * 
	 * @return Port for TCP connection.
	 */
	public int getPort() {
		return mPort;
	}
		
	/**
	 * Gets value of IP address.
	 * 
	 * @return IP address for TCP connection.
	 */
	public String getIPAddress() {
		return mIpAddress;
	}

    /**
     * Gets value of AutoReconnect
     * @return Flag that determines automatic reconnection
     */
    public boolean getAutoReconnect() {
        return mAutoReconnect;
    }

    /**
	 * Overridden abstract method which returns specific type of this transport configuration.
	 * 
	 * @return Constant value TransportType.TCP. 
	 * 
	 * @see TransportType
	 */
	public TransportType getTransportType() {
		return TransportType.TCP;
	}

    @Override
    public String toString() {
        return "TCPTransportConfig{" +
                "Port=" + mPort +
                ", IpAddress='" + mIpAddress + '\'' +
                ", AutoReconnect=" + mAutoReconnect +
                '}';
    }
}
