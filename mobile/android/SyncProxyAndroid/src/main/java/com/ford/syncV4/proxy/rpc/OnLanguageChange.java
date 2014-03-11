package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.util.DebugTool;

public class OnLanguageChange extends RPCNotification {
	
    public OnLanguageChange() {
        super("OnLanguageChange");
    }
    public OnLanguageChange(Hashtable hash) {
        super(hash);
    }
    public void setLanguage(Language language) {
        if (language != null) {
            parameters.put(Names.language, language);
        } else {
        	parameters.remove(Names.language);
        }
    }
    public Language getLanguage() {
    	Object obj = parameters.get(Names.language);
        if (obj instanceof Language) {
            return (Language) obj;
        } else if (obj instanceof String) {
        	Language theCode = null;
            try {
                theCode = Language.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.language, e);
            }
            return theCode;
        }
        return null;
    }
    public void setHmiDisplayLanguage(Language hmiDisplayLanguage) {
        if (hmiDisplayLanguage != null) {
            parameters.put(Names.hmiDisplayLanguage, hmiDisplayLanguage);
        } else {
        	parameters.remove(Names.hmiDisplayLanguage);
        }
    }
    public Language getHmiDisplayLanguage() {
    	Object obj = parameters.get(Names.hmiDisplayLanguage);
        if (obj instanceof Language) {
            return (Language) obj;
        } else if (obj instanceof String) {
        	Language theCode = null;
            try {
                theCode = Language.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.hmiDisplayLanguage, e);
            }
            return theCode;
        }
        return null;
    }
}
