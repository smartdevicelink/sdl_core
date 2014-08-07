package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.AppHMIType;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.util.logger.Logger;

/**
 * Establishes an interface with a mobile application.
 * Before {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface}
 * no other commands will be accepted / executed.
 */
public class RegisterAppInterface extends RPCRequest {

    private static final String LOG_TAG = RegisterAppInterface.class.getSimpleName();
    private static final int HASH_ID_MAX_LENGTH = 100;

    /**
     * Constructor
     */
    public RegisterAppInterface() {
        super(Names.RegisterAppInterface);
    }

    /**
     * Constructor
     *
     * @param hash hash table of the {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface}
     *             structure
     */
    public RegisterAppInterface(Hashtable hash) {
        super(hash);
    }

    /**
     * See {@link com.ford.syncV4.proxy.rpc.SyncMsgVersion}
     *
     * @return instance of {@link com.ford.syncV4.proxy.rpc.SyncMsgVersion}
     */
    public SyncMsgVersion getSyncMsgVersion() {
        Object obj = parameters.get(Names.syncMsgVersion);
        if (obj instanceof SyncMsgVersion) {
            return (SyncMsgVersion) obj;
        } else if (obj instanceof Hashtable) {
            return new SyncMsgVersion((Hashtable) obj);
        }
        return null;
    }

    /**
     * Set {@link com.ford.syncV4.proxy.rpc.SyncMsgVersion}
     *
     * @param syncMsgVersion instance of {@link com.ford.syncV4.proxy.rpc.SyncMsgVersion}
     */
    public void setSyncMsgVersion(SyncMsgVersion syncMsgVersion) {
        if (syncMsgVersion != null) {
            parameters.put(Names.syncMsgVersion, syncMsgVersion);
        } else {
            parameters.remove(Names.syncMsgVersion);
        }
    }

    /**
     * The mobile application name, e.g. "Ford Drive Green".
     * Needs to be unique over all applications.
     * May not be empty.
     * May not start with a new line character.
     * May not interfere with any name or synonym of previously registered applications and any
     * predefined blacklist of words (global commands)
     * Needs to be unique over all applications. Applications with the same name will be rejected.
     * Only characters from char set
     * [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
     *
     * @return application name
     */
    public Object getAppName() {
        return parameters.get(Names.appName);
    }

    /**
     * The mobile application name, e.g. "Ford Drive Green".
     * Needs to be unique over all applications.
     * May not be empty.
     * May not start with a new line character.
     * May not interfere with any name or synonym of previously registered applications and any
     * predefined blacklist of words (global commands)
     * Needs to be unique over all applications. Applications with the same name will be rejected.
     * Only characters from char set
     * [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
     *
     * @param appName application name
     */
    public void setAppName(Object appName) {
        if (appName != null) {
            parameters.put(Names.appName, appName);
        } else {
            parameters.remove(Names.appName);
        }
    }

    /**
     * TTS string for VR recognition of the mobile application name, e.g. "Ford Drive Green".
     * Meant to overcome any failing on speech engine in properly pronouncing / understanding app name.
     * Needs to be unique over all applications.
     * May not be empty.
     * May not start with a new line character.
     * Only characters from char set
     * [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
     *
     * @return array of the {@link com.ford.syncV4.proxy.rpc.TTSChunk}'s
     */
    public Vector<TTSChunk> getTtsName() {
        if (parameters.get(Names.ttsName) instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) parameters.get(Names.ttsName);
            if (list == null) {
                return null;
            }
            if (list.size() == 0) {
                return (Vector<TTSChunk>) list;
            }
            Object obj = list.get(0);
            if (obj instanceof TTSChunk) {
                return (Vector<TTSChunk>) list;
            } else if (obj instanceof Hashtable) {
                Vector<TTSChunk> newList = new Vector<TTSChunk>();
                for (Object hashObj : list) {
                    newList.add(new TTSChunk((Hashtable) hashObj));
                }
                return newList;
            }
        }
        return null;
    }

    /**
     * TTS string for VR recognition of the mobile application name, e.g. "Ford Drive Green".
     * Meant to overcome any failing on speech engine in properly pronouncing / understanding app name.
     * Needs to be unique over all applications.
     * May not be empty.
     * May not start with a new line character.
     * Only characters from char set
     * [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
     *
     * @param ttsName array of the {@link com.ford.syncV4.proxy.rpc.TTSChunk}'s
     */
    public void setTtsName(Vector<TTSChunk> ttsName) {
        if (ttsName != null) {
            parameters.put(Names.ttsName, ttsName);
        } else {
            parameters.remove(Names.ttsName);
        }
    }

    /**
     * Provides an abbreviated version of the app name (if needed),
     * that will be displayed on the NGN media screen.
     * If not provided, the appName is used instead (and will be truncated if too long)
     * Only characters from char set
     * [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
     *
     * @return abbreviated version of the app name
     */
    public Object getNgnMediaScreenAppName() {
        return parameters.get(Names.ngnMediaScreenAppName);
    }

    /**
     * Provides an abbreviated version of the app name (if needed),
     * that will be displayed on the NGN media screen.
     * If not provided, the appName is used instead (and will be truncated if too long)
     * Only characters from char set
     * [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
     *
     * @param ngnMediaScreenAppName abbreviated version of the app name
     */
    public void setNgnMediaScreenAppName(Object ngnMediaScreenAppName) {
        if (ngnMediaScreenAppName != null) {
            parameters.put(Names.ngnMediaScreenAppName, ngnMediaScreenAppName);
        } else {
            parameters.remove(Names.ngnMediaScreenAppName);
        }
    }

    /**
     * Defines an additional voice recognition command.
     * May not interfere with any app name of previously registered applications and any
     * predefined blacklist of words (global commands)
     * Only characters from char set
     * [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
     *
     * @return additional voice recognition command
     */
    public Vector<Object> getVrSynonyms() {
        if (parameters.get(Names.vrSynonyms) instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) parameters.get(Names.vrSynonyms);
            if (list == null) {
                return null;
            }
            if (list.size() == 0) {
                return (Vector<Object>) list;
            }
            Object obj = list.get(0);
            if (obj instanceof Object) {
                return (Vector<Object>) list;
            }
        }
        return null;
    }

    /**
     * Defines an additional voice recognition command.
     * May not interfere with any app name of previously registered applications and any
     * predefined blacklist of words (global commands)
     * Only characters from char set
     * [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
     *
     * @param vrSynonyms additional voice recognition command
     */
    public void setVrSynonyms(Vector<Object> vrSynonyms) {
        if (vrSynonyms != null) {
            parameters.put(Names.vrSynonyms, vrSynonyms);
        } else {
            parameters.remove(Names.vrSynonyms);
        }
    }

    /**
     * Indicates if the application is a media or a non-media application.
     * Only media applications will be able to stream audio to Sync that is audible outside
     * of the BT media source.
     *
     * @return true or false
     */
    public Object getIsMediaApplication() {
        return parameters.get(Names.isMediaApplication);
    }

    /**
     * Indicates if the application is a media or a non-media application.
     * Only media applications will be able to stream audio to Sync that is audible outside
     * of the BT media source.
     *
     * @param isMediaApplication true or false
     */
    public void setIsMediaApplication(Object isMediaApplication) {
        if (isMediaApplication != null) {
            parameters.put(Names.isMediaApplication, isMediaApplication);
        } else {
            parameters.remove(Names.isMediaApplication);
        }
    }

    /**
     * See {@link com.ford.syncV4.proxy.rpc.enums.Language}
     * Current app’s expected VR+TTS language
     * If there is a mismatch with SYNC, the app will be able to change this registration with
     * changeRegistration prior to app being brought into focus.
     *
     * @return {@link com.ford.syncV4.proxy.rpc.enums.Language}
     */
    public Language getLanguageDesired() {
        Object obj = parameters.get(Names.languageDesired);
        if (obj instanceof Language) {
            return (Language) obj;
        } else if (obj instanceof String) {
            Language theCode = null;
            try {
                theCode = Language.valueForString((String) obj);
            } catch (Exception e) {
                Logger.e("Failed to parse " + getClass().getSimpleName() + "." + Names.languageDesired, e);
            }
            return theCode;
        }
        return null;
    }

    /**
     * See {@link com.ford.syncV4.proxy.rpc.enums.Language}
     * Current app’s expected VR+TTS language
     * If there is a mismatch with SYNC, the app will be able to change this registration with
     * changeRegistration prior to app being brought into focus.
     *
     * @param languageDesired {@link com.ford.syncV4.proxy.rpc.enums.Language}
     */
    public void setLanguageDesired(Language languageDesired) {
        if (languageDesired != null) {
            parameters.put(Names.languageDesired, languageDesired);
        } else {
            parameters.remove(Names.languageDesired);
        }
    }

    /**
     * See {@link com.ford.syncV4.proxy.rpc.enums.Language}
     * Current app’s expected display language
     * If there is a mismatch with SYNC, the app will be able to change this registration with
     * changeRegistration prior to app being brought into focus.
     *
     * @return {@link com.ford.syncV4.proxy.rpc.enums.Language}
     */
    public Language getHmiDisplayLanguageDesired() {
        Object obj = parameters.get(Names.hmiDisplayLanguageDesired);
        if (obj instanceof Language) {
            return (Language) obj;
        } else if (obj instanceof String) {
            Language theCode = null;
            try {
                theCode = Language.valueForString((String) obj);
            } catch (Exception e) {
                Logger.e("Failed to parse " + getClass().getSimpleName() + "." + Names.hmiDisplayLanguageDesired, e);
            }
            return theCode;
        }
        return null;
    }

    /**
     * See {@link com.ford.syncV4.proxy.rpc.enums.Language}
     * Current app’s expected display language
     * If there is a mismatch with SYNC, the app will be able to change this registration with
     * changeRegistration prior to app being brought into focus.
     *
     * @param hmiDisplayLanguageDesired {@link com.ford.syncV4.proxy.rpc.enums.Language}
     */
    public void setHmiDisplayLanguageDesired(Language hmiDisplayLanguageDesired) {
        if (hmiDisplayLanguageDesired != null) {
            parameters.put(Names.hmiDisplayLanguageDesired, hmiDisplayLanguageDesired);
        } else {
            parameters.remove(Names.hmiDisplayLanguageDesired);
        }
    }

    /**
     * See {@link com.ford.syncV4.proxy.rpc.enums.AppHMIType}
     * List of all applicable app HMI types stating which HMI classifications to be given to the app.
     *
     * @return {@link com.ford.syncV4.proxy.rpc.enums.AppHMIType}
     */
    public Vector<AppHMIType> getAppType() {
        if (parameters.get(Names.appHMIType) instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) parameters.get(Names.appHMIType);
            if (list == null) {
                return null;
            }
            if (list.size() == 0) {
                return (Vector<AppHMIType>) list;
            }
            Object obj = list.get(0);
            if (obj instanceof AppHMIType) {
                return (Vector<AppHMIType>) list;
            } else if (obj instanceof String) {
                Vector<AppHMIType> newList = new Vector<AppHMIType>();
                for (Object hashObj : list) {
                    String strFormat = (String) hashObj;
                    AppHMIType toAdd = null;
                    try {
                        toAdd = AppHMIType.valueForString(strFormat);
                    } catch (Exception e) {
                        Logger.e("Failed to parse " + getClass().getSimpleName() + "." + Names.appHMIType, e);
                    }
                    if (toAdd != null) {
                        newList.add(toAdd);
                    }
                }
                return newList;
            }
        }
        return null;
    }

    /**
     * See {@link com.ford.syncV4.proxy.rpc.enums.AppHMIType}
     * List of all applicable app HMI types stating which HMI classifications to be given to the app.
     *
     * @param appHMIType {@link com.ford.syncV4.proxy.rpc.enums.AppHMIType}
     */
    public void setAppType(Vector<AppHMIType> appHMIType) {
        if (appHMIType != null) {
            parameters.put(Names.appHMIType, appHMIType);
        } else {
            parameters.remove(Names.appHMIType);
        }
    }

    /**
     * ID used to validate app with policy table entries
     *
     * @return Application Id
     */
    public Object getAppId() {
        return parameters.get(Names.appID);
    }

    /**
     * ID used to validate app with policy table entries
     *
     * @param appID Application Id
     */
    public void setAppId(Object appID) {
        if (appID != null) {
            parameters.put(Names.appID, appID);
        } else {
            parameters.remove(Names.appID);
        }
    }

    /**
     * Return uniquely identify of the current state of all app data that can persist through
     * connection cycles.
     * <p/>
     * ID used to uniquely identify current state of all app data that can persist through
     * connection cycles (e.g. ignition cycles). This registered data (commands, submenus,
     * choice sets, etc.) can be reestablished without needing to explicitly reregister each piece.
     * If omitted, then the previous state of an app's commands, etc. will not be restored.
     * When sending hashID, all RegisterAppInterface parameters should still be provided
     * (e.g. ttsName, etc.).
     *
     * @return {@link java.lang.String} uniquely identify of the current state of all app data
     */
    public String getHashID() {
        if (!parameters.containsKey(Names.hashID)) {
            return null;
        }
        if (parameters.get(Names.hashID) instanceof String) {
            return (String) parameters.get(Names.hashID);
        }
        return null;
    }

    /**
     * Set uniquely identify of the current state of all app data that can persist through
     * connection cycles.
     * <p/>
     * ID used to uniquely identify current state of all app data that can persist through
     * connection cycles (e.g. ignition cycles). This registered data (commands, submenus,
     * choice sets, etc.) can be reestablished without needing to explicitly reregister each piece.
     * If omitted, then the previous state of an app's commands, etc. will not be restored.
     * When sending hashID, all RegisterAppInterface parameters should still be provided
     * (e.g. ttsName, etc.).
     *
     * @param hashID uniquely identify of the current state
     */
    public void setHashID(String hashID) {
        if (hashID != null) {
            if (hashID.length() > HASH_ID_MAX_LENGTH) {
                throw new IllegalArgumentException("Parameter 'hashId' can have max length " +
                        "'" + HASH_ID_MAX_LENGTH + "', current length is '" + hashID.length() + "'");
            }
            parameters.put(Names.hashID, hashID);
        } else {
            parameters.remove(Names.hashID);
        }
    }

    /**
     * Set the object which holds various information about connecting device
     * @param deviceInfo {@link com.ford.syncV4.proxy.rpc.DeviceInfo}
     */
    public void setDeviceInfo(DeviceInfo deviceInfo) {
        if (deviceInfo != null) {
            parameters.put(Names.deviceInfo, deviceInfo);
        } else {
            parameters.remove(Names.deviceInfo);
        }
    }

    /**
     * Return the object which holds various information about connecting device
     * @return {@link com.ford.syncV4.proxy.rpc.DeviceInfo}
     */
    public DeviceInfo getDeviceInfo() {
        Object object = parameters.get(Names.deviceInfo);
        if (object instanceof DeviceInfo) {
            return (DeviceInfo) object;
        } else if (object instanceof Hashtable) {
            return new DeviceInfo((Hashtable) object);
        }
        Logger.w(LOG_TAG + " getDeviceInfo is not a type of DeviceInfo, current value:" + object);
        return null;
    }
}