/**
 * 
 */
package com.ford.syncV4.proxy.rpc.enums;

/**
 * @author skhare3
 *Enumeration that describes possible states of driver distraction.
 */
public enum DriverDistractionState {
	DD_ON,
	DD_OFF;
	
	public static DriverDistractionState valueForString(String value) {
    	return valueOf(value);
    }
}
