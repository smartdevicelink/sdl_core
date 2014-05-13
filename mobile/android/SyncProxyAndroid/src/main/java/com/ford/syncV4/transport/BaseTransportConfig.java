package com.ford.syncV4.transport;

/**
 * Defines base abstract class for transport configurations.
 */
public abstract class BaseTransportConfig {

    protected boolean shareConnection = true;

	/**
	 * Gets transport type for this transport configuration.
	 * 
	 * @return One of {@link TransportType} enumeration values that represents type of this
     * transport configuration.
	 */
	public abstract TransportType getTransportType();

    /**
     * Indicate whether the application want to share connection with others.
     *
     * @return
     */
    public boolean shareConnection() {
        return shareConnection;
    }
}