package com.ford.syncV4.transport;

import android.content.Context;

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
     * Indicates whether TCP connection use Network Service Discovery (NSD) or not. NSD is used
     * for the case of SDL using any available devices to connect to.
     */
    private Boolean mIsNSD;

    /**
     * Context of the application which will use TCP NSD.
     */
    private Context mApplicationContext;

	/**
	 * Constructor. Objects of this class must be created for known port and IP address value.
	 *
	 * @param port Port for TCP connection.
	 * @param ipAddress IP address for TCP connection.
	 */
	public TCPTransportConfig(int port, String ipAddress) {
		mPort = port;
		mIpAddress = ipAddress;
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
	 * Overridden abstract method which returns specific type of this transport configuration.
	 * 
	 * @return Constant value TransportType.TCP. 
	 * 
	 * @see TransportType
	 */
	public TransportType getTransportType() {
		return TransportType.TCP;
	}

    /**
     * Get whether TCP connection use Network Service Discovery (NSD) or not.
     * @return Value of the isNSD field
     */
    public Boolean getIsNSD() {
        return mIsNSD;
    }

    /**
     * Set whether TCP connection use Network Service Discovery (NSD) or not.
     * @param mIsNSD
     */
    public void setIsNSD(Boolean mIsNSD) {
        this.mIsNSD = mIsNSD;
    }

    public Context getApplicationContext() {
        return mApplicationContext;
    }

    public void setApplicationContext(Context mApplicationContext) {
        this.mApplicationContext = mApplicationContext;
    }

    @Override
    public String toString() {
        return "TCPTransportConfig{Port=" + mPort + ", IpAddress=" + mIpAddress + ", isNSD=" +
                mIsNSD + ", context: " + mApplicationContext + "}";
    }
}