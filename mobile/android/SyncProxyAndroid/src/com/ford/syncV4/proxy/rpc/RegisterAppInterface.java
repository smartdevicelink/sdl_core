package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
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
        	return (SyncMsgVersion)obj;
        } else if (obj instanceof Hashtable) {
        	return new SyncMsgVersion((Hashtable)obj);
        }
        return null;
    }
    public void setSyncMsgVersion( SyncMsgVersion syncMsgVersion ) {
        if (syncMsgVersion != null) {
            parameters.put(Names.syncMsgVersion, syncMsgVersion );
        }
    }
    public String getAppName() {
        return (String) parameters.get( Names.appName );
    }
    public void setAppName( String appName ) {
        if (appName != null) {
            parameters.put(Names.appName, appName );
        }
    }
    public String getNgnMediaScreenAppName() {
        return (String) parameters.get( Names.ngnMediaScreenAppName );
    }
    public void setNgnMediaScreenAppName( String ngnMediaScreenAppName ) {
        if (ngnMediaScreenAppName != null) {
            parameters.put(Names.ngnMediaScreenAppName, ngnMediaScreenAppName );
        }
    }
    public Vector<String> getVrSynonyms() {
    	if (parameters.get( Names.vrSynonyms ) instanceof Vector<?>) {
    		Vector<?> list = (Vector<?>)parameters.get( Names.vrSynonyms );
    		if (list != null && list.size()>0) {
    			Object obj = list.get(0);
    			if (obj instanceof String) {
    				return (Vector<String>) list;
    			}
    		}
    	}
        return null;
    }
    public void setVrSynonyms( Vector<String> vrSynonyms ) {
        if (vrSynonyms != null) {
            parameters.put(Names.vrSynonyms, vrSynonyms );
        }
    }
    public Boolean getIsMediaApplication() {
        return (Boolean) parameters.get( Names.isMediaApplication );
    }
    public void setIsMediaApplication( Boolean isMediaApplication ) {
        if (isMediaApplication != null) {
            parameters.put(Names.isMediaApplication, isMediaApplication );
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
    public void setLanguageDesired( Language languageDesired ) {
        if (languageDesired != null) {
            parameters.put(Names.languageDesired, languageDesired );
        }
    }
    public String getAutoActivateID() {
        return (String) parameters.get( Names.autoActivateID );
    }
    public void setAutoActivateID( String autoActivateID ) {
        if (autoActivateID != null) {
            parameters.put(Names.autoActivateID, autoActivateID );
        }
    }
    
    public void setHmiDisplayLanguageDesired(String hmiLanguage) {
    	if (hmiLanguage != null) {
    		parameters.put("hmiDisplayLanguageDesired", hmiLanguage);
    	}
    }
    
    public void setAppId(String id){
    	parameters.put("appID", id);
    }
}
