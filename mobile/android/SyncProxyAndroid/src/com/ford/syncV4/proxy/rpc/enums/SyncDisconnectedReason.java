package com.ford.syncV4.proxy.rpc.enums;

public enum SyncDisconnectedReason {
	USER_EXIT,
    IGNITION_OFF,
    BLUETOOTH_OFF,
    USB_DISCONNECTED,
    REQUEST_WHILE_IN_NONE_HMI_LEVEL,
    TOO_MANY_REQUESTS,
    DRIVER_DISTRACTION_VIOLATION,
    LANGUAGE_CHANGE,
    MASTER_RESET,
    FACTORY_DEFAULTS,
    HEARTBEAT_PAST_DUE,
    TRANSPORT_ERROR,
    APPLICATION_REQUESTED_DISCONNECT,
    DEFAULT;
	
	public static SyncDisconnectedReason valueForString(String value) {
        return valueOf(value);
    }
	
	public static SyncDisconnectedReason convertAppInterfaceUnregisteredReason(AppInterfaceUnregisteredReason reason) {
		
		SyncDisconnectedReason returnReason = SyncDisconnectedReason.DEFAULT;
		
		switch(reason) {
			case USER_EXIT:
				returnReason = SyncDisconnectedReason.USER_EXIT;
			case IGNITION_OFF:
				returnReason = SyncDisconnectedReason.IGNITION_OFF;
			case BLUETOOTH_OFF:
				returnReason = SyncDisconnectedReason.BLUETOOTH_OFF;
			case USB_DISCONNECTED:
				returnReason = SyncDisconnectedReason.USB_DISCONNECTED;
			case REQUEST_WHILE_IN_NONE_HMI_LEVEL:
				returnReason = SyncDisconnectedReason.REQUEST_WHILE_IN_NONE_HMI_LEVEL;
			case TOO_MANY_REQUESTS:
				returnReason = SyncDisconnectedReason.TOO_MANY_REQUESTS;
			case DRIVER_DISTRACTION_VIOLATION:
				returnReason = SyncDisconnectedReason.DRIVER_DISTRACTION_VIOLATION;
			case LANGUAGE_CHANGE:
				returnReason = SyncDisconnectedReason.LANGUAGE_CHANGE;
			case MASTER_RESET:
				returnReason = SyncDisconnectedReason.MASTER_RESET;
			case FACTORY_DEFAULTS:
				returnReason = SyncDisconnectedReason.FACTORY_DEFAULTS;
		}
		
		return returnReason;
	}
}
