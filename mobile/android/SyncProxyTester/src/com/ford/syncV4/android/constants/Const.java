package com.ford.syncV4.android.constants;

/** Stores application-wide constants. */
public class Const {
	// Shared preference name for protocol properties
	public static final String PREFS_NAME = "SyncProxyTesterPrefs";
	
	// Protocol properties
	public static final String PREFS_KEY_PROTOCOLVERSION = "VersionNumber";
	public static final String PREFS_KEY_ISMEDIAAPP = "isMediaApp";
	
	// Default values
	public static final int PREFS_DEFAULT_PROTOCOLVERSION = 1;
	public static final boolean PREFS_DEFAULT_ISMEDIAAPP = true;
}
