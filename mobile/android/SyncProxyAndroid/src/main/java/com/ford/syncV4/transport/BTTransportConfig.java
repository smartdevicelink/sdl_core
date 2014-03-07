package com.ford.syncV4.transport;

/**
 * Container of Bluetooth transport specific configuration.
 */
public final class BTTransportConfig extends BaseTransportConfig {

	/**
	 * Overridden abstract method which returns specific type of this transport configuration.
	 * 
	 * @return Constant value TransportType.BLUETOOTH. 
	 * 
	 * @see TransportType
	 */
	public TransportType getTransportType() {
		return TransportType.BLUETOOTH;
	}

}
