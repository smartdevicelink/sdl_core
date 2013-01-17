package com.ford.syncV4.transport;

/**
 * Defines base abstract class for transport configurations.
 */
public abstract class BaseTransportConfig {
	
	/**
	 * Gets transport type for this transport configuration.
	 * 
	 * @return One of {@link TransportType} enumeration values that represents type of this transport configuration.
	 */
	public abstract TransportType getTransportType();
}
