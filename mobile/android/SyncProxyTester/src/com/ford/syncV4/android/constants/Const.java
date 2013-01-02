package com.ford.syncV4.android.constants;

import com.ford.syncV4.proxy.rpc.enums.Language;

/** Stores application-wide constants. */
public class Const {
	// Shared preference name for protocol properties
	public static final String PREFS_NAME = "SyncProxyTesterPrefs";

	// Protocol properties
	public static final String PREFS_KEY_PROTOCOLVERSION = "VersionNumber";
	public static final String PREFS_KEY_ISMEDIAAPP = "isMediaApp";
	public static final String PREFS_KEY_APPNAME = "appName";
	public static final String PREFS_KEY_LANG = "desiredLang";
	public static final String PREFS_KEY_HMILANG = "desiredHMILang";

	// Default values
	public static final int PREFS_DEFAULT_PROTOCOLVERSION = 1;
	public static final boolean PREFS_DEFAULT_ISMEDIAAPP = true;
	public static final String PREFS_DEFAULT_APPNAME = "SyncProxyTester";
	public static final String PREFS_DEFAULT_LANG = Language.EN_US.name();
	public static final String PREFS_DEFAULT_HMILANG = Language.EN_US.name();

	// Keys to pass objects via IntentHelper
	public static final String INTENTHELPER_KEY_SOFTBUTTON = "IntentSoftButton";
	public static final String INTENTHELPER_KEY_SOFTBUTTONSLIST = "IntentSoftButtonsList";

	// Request id for SoftButtonEditActivity
	public static final int REQUEST_EDIT_SOFTBUTTON = 42;
	// Request id for SoftButtonsListActivity
	public static final int REQUEST_LIST_SOFTBUTTONS = 43;
}
