package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.AppHMIType;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.util.DebugTool;

public class RegisterAppInterface extends RPCRequest {

    public RegisterAppInterface() {
        super("RegisterAppInterface");
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
	    	Vector<?> list = (Vector<?>)parameters.get(Names.ttsName);
	        if (list != null && list.size() > 0) {
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
    		Vector<?> list = (Vector<?>)parameters.get(Names.vrSynonyms);
    		if (list != null && list.size()>0) {
    			Object obj = list.get(0);
    			if (obj instanceof String) {
    				return (Vector<String>) list;
    			}
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
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.languageDesired, e);
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
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.hmiDisplayLanguageDesired, e);
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
	    	Vector<?> list = (Vector<?>)parameters.get(Names.appHMIType);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof AppHMIType) {
	                return (Vector<AppHMIType>) list;
	            } else if (obj instanceof String) {
	                Vector<AppHMIType> newList = new Vector<AppHMIType>();
	                for (Object hashObj : list) {
	                    String strFormat = (String)hashObj;
	                    AppHMIType toAdd = null;
	                    try {
	                        toAdd = AppHMIType.valueForString(strFormat);
	                    } catch (Exception e) {
	                    	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.appHMIType, e);	                    }
	                    if (toAdd != null) {
	                        newList.add(toAdd);
	                    }
	                }
	                return newList;
	            }
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
}
