package com.ford.syncV4.proxy.rpc.enums;

public enum PermissionStatus {
	ALLOWED,
	DISALLOWED,
	USER_DISALLOWED,
	USER_CONSENT_PENDING;
	
	public static PermissionStatus valueForString(String value) {
        return valueOf(value);
    }
}
