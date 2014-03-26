package com.ford.syncV4.android.constants;

import com.ford.syncV4.proxy.rpc.enums.Language;

/** Stores application-wide constants. */
public class Const {
	// Shared preference name for protocol properties
	public static final String PREFS_NAME = "SyncProxyTesterPrefs";

	// Protocol properties
	public static final String PREFS_KEY_ISMEDIAAPP = "isMediaApp";
    public static final String PREFS_KEY_ISNAVIAPP = "isNaviApp";
    public static final String PREFS_KEY_NAVI_VIDEOSOURCE = "videoSrc";
	public static final String PREFS_KEY_APPNAME = "appName";
	public static final String PREFS_KEY_LANG = "desiredLang";

	public static final String PREFS_KEY_HMILANG = "desiredHMILang";
	public static final String PREFS_KEY_AUTOSETAPPICON = "autoSetAppIcon";
	public static final String PREFS_KEY_DISABLE_LOCK_WHEN_TESTING = "disableLockWhenTesting";

    public static final int KEY_VIDEOSOURCE_MP4 = 0;
    public static final int KEY_VIDEOSOURCE_H264 = 1;

	// Default values
	public static final boolean PREFS_DEFAULT_ISMEDIAAPP = true;
    public static final boolean PREFS_DEFAULT_ISNAVIAPP = true;
    public static final int PREFS_DEFAULT_NAVI_VIDEOSOURCE = KEY_VIDEOSOURCE_MP4;
	public static final String PREFS_DEFAULT_APPNAME = FlavorConst.PREFS_DEFAULT_APPNAME;

    public static final String PREFS_DEFAULT_LANG = Language.EN_US.name();
	public static final String PREFS_DEFAULT_HMILANG = Language.EN_US.name();
	public static final boolean PREFS_DEFAULT_AUTOSETAPPICON = true;
	public static final boolean PREFS_DEFAULT_DISABLE_LOCK_WHEN_TESTING = false;

    // Protocol version constants
    public static final int PROTOCOL_VERSION_1 = 1;
    public static final int PROTOCOL_VERSION_2 = 2;

	// Transport properties
	public static final class Transport {
		// Protocol properties
		public static final String PREFS_KEY_TRANSPORT_TYPE = "TransportType";
		public static final String PREFS_KEY_TRANSPORT_PORT = "TCPPort";
		public static final String PREFS_KEY_TRANSPORT_IP = "IPAddress";
		public static final String PREFS_KEY_IS_NSD = "IsNSD";

		public static final String TCP = "WiFi";
		public static final String BLUETOOTH = "Bluetooth";
		public static final String USB = "USB";
		public static final int KEY_UNKNOWN = -1;
		public static final int KEY_TCP = 1;
		public static final int KEY_BLUETOOTH = 2;
		public static final int KEY_USB = 3;

		public static final int PREFS_DEFAULT_TRANSPORT_TYPE = KEY_USB;
		public static final int PREFS_DEFAULT_TRANSPORT_PORT = 12345;
		public static final String PREFS_DEFAULT_TRANSPORT_IP = "10.10.0.1";
	}

    // Policy properties
    public static final class Policy {
        public static final String PREF_KEY_POLICY_UPDATE_FILE_PATH = "PolicyUpdateFilePath";
        public static final String PREF_KEY_POLICY_UPDATE_AUTO_REPLAY = "PolicyUpdateAutoReplay";
    }

    // HashId properties
    public static final class HashId {
        public static final String PREF_KEY_USE_HASH_ID = "UseHashId";
        public static final String PREF_KEY_USE_CUSTOM_HASH_ID = "UseCustomHashId";
        public static final String PREF_KEY_CUSTOM_HASH_ID = "CustomHashId";
        public static final String PREF_KEY_LAST_HASH_IDS = "LastHashIds";
    }

	// Keys to pass objects via IntentHelper
	public static final String INTENTHELPER_KEY_OBJECT = "IntentObject";
	public static final String INTENTHELPER_KEY_OBJECTSLIST = "IntentObjectsList";
    public static final String INTENTHELPER_KEY_KEYBOARDPROPERTIES = "IntentKeyboardProperties";
    public static final String INTENTHELPER_KEY_KEYBOARDPROPERTIES_EMPTY = "IntentKeyboardPropertiesEmpty";

	// Keys to pass values via Intent
	public static final String INTENT_KEY_OBJECTS_MAXNUMBER = "MaxObjectsNumber";

    // Request id for KeyboardPropertiesActivity
	public static final int REQUEST_EDIT_KBDPROPERTIES = 44;

    // Value of the Jellybean API level, to check on devices running API level lower then API 16
    public static final int JELLYBEAN_API_LEVEL = 16;

    public static final int REQUEST_FILE_OPEN = 50;
    public static final int REQUEST_POLICY_UPDATE_FILE_OPEN = 51;

    public static final String PREF_KEY_IS_CUSTOM_APP_ID = "IsCustomAppId";
    public static final String PREF_KEY_CUSTOM_APP_ID = "CustomAppId";
}
