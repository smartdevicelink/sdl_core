package com.ford.syncV4.proxy.rpc;

import android.util.Log;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.AppHMIType;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.util.logger.Logger;

public class RegisterAppInterface extends RPCRequest {

    private static final String LOG_TAG = RegisterAppInterface.class.getSimpleName();
    private static final int HASH_ID_MAX_LENGTH = 100;

    public RegisterAppInterface() {
        super(Names.RegisterAppInterface);
    }

    public RegisterAppInterface(Hashtable hash) {
        super(hash);
    }

    public SyncMsgVersion getSyncMsgVersion() {
        Object obj = parameters.get(Names.syncMsgVersion);
        if (obj instanceof SyncMsgVersion) {
            return (SyncMsgVersion) obj;
        } else if (obj instanceof Hashtable) {
            return new SyncMsgVersion((Hashtable) obj);
        }
        return null;
    }

    public void setSyncMsgVersion(SyncMsgVersion syncMsgVersion) {
        if (syncMsgVersion != null) {
            parameters.put(Names.syncMsgVersion, syncMsgVersion);
        } else {
            parameters.remove(Names.syncMsgVersion);
        }
    }

    public String getAppName() {
        return (String) parameters.get(Names.appName);
    }

    public void setAppName(String appName) {
        if (appName != null) {
            parameters.put(Names.appName, appName);
        } else {
            parameters.remove(Names.appName);
        }
    }

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

    public void setTtsName(Vector<TTSChunk> ttsName) {
        if (ttsName != null) {
            parameters.put(Names.ttsName, ttsName);
        } else {
            parameters.remove(Names.ttsName);
        }
    }

    public String getNgnMediaScreenAppName() {
        return (String) parameters.get(Names.ngnMediaScreenAppName);
    }

    public void setNgnMediaScreenAppName(String ngnMediaScreenAppName) {
        if (ngnMediaScreenAppName != null) {
            parameters.put(Names.ngnMediaScreenAppName, ngnMediaScreenAppName);
        } else {
            parameters.remove(Names.ngnMediaScreenAppName);
        }
    }

    public Vector<String> getVrSynonyms() {
        if (parameters.get(Names.vrSynonyms) instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) parameters.get(Names.vrSynonyms);
            if (list == null) {
                return null;
            }
            if (list.size() == 0) {
                return (Vector<String>) list;
            }
            Object obj = list.get(0);
            if (obj instanceof String) {
                return (Vector<String>) list;
            }
        }
        return null;
    }

    public void setVrSynonyms(Vector<String> vrSynonyms) {
        if (vrSynonyms != null) {
            parameters.put(Names.vrSynonyms, vrSynonyms);
        } else {
            parameters.remove(Names.vrSynonyms);
        }
    }

    public Boolean getIsMediaApplication() {
        return (Boolean) parameters.get(Names.isMediaApplication);
    }

    public void setIsMediaApplication(Boolean isMediaApplication) {
        if (isMediaApplication != null) {
            parameters.put(Names.isMediaApplication, isMediaApplication);
        } else {
            parameters.remove(Names.isMediaApplication);
        }
    }

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

    public void setLanguageDesired(Language languageDesired) {
        if (languageDesired != null) {
            parameters.put(Names.languageDesired, languageDesired);
        } else {
            parameters.remove(Names.languageDesired);
        }
    }

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

    public void setHmiDisplayLanguageDesired(Language hmiDisplayLanguageDesired) {
        if (hmiDisplayLanguageDesired != null) {
            parameters.put(Names.hmiDisplayLanguageDesired, hmiDisplayLanguageDesired);
        } else {
            parameters.remove(Names.hmiDisplayLanguageDesired);
        }
    }

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

    public void setAppType(Vector<AppHMIType> appHMIType) {
        if (appHMIType != null) {
            parameters.put(Names.appHMIType, appHMIType);
        } else {
            parameters.remove(Names.appHMIType);
        }
    }

    public String getAppID() {
        return (String) parameters.get(Names.appID);
    }

    public void setAppID(String appID) {
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
        Log.w(LOG_TAG, " getDeviceInfo is not a type of DeviceInfo, current value:" + object);
        return null;
    }
}