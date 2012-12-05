package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.HmiZoneCapabilities;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.SpeechCapabilities;
import com.ford.syncV4.proxy.rpc.enums.VrCapabilities;
import com.ford.syncV4.util.DebugTool;

public class RegisterAppInterfaceResponse extends RPCResponse {

    public RegisterAppInterfaceResponse() {
        super("RegisterAppInterface");
    }
    public RegisterAppInterfaceResponse(Hashtable hash) {
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
    public String getAutoActivateID() {
        return (String) parameters.get( Names.autoActivateID );
    }
    public void setAutoActivateID( String autoActivateID ) {
        if (autoActivateID != null) {
            parameters.put(Names.autoActivateID, autoActivateID );
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
    public void setLanguage( Language language ) {
        if (language != null) {
            parameters.put(Names.language, language );
        }
    }
    public DisplayCapabilities getDisplayCapabilities() {
        Object obj = parameters.get(Names.displayCapabilities);
        if (obj instanceof DisplayCapabilities) {
        	return (DisplayCapabilities)obj;
        } else if (obj instanceof Hashtable) {
        	return new DisplayCapabilities((Hashtable)obj);
        }
        return null;
    }
    public void setDisplayCapabilities( DisplayCapabilities displayCapabilities ) {
        if (displayCapabilities != null) {
            parameters.put(Names.displayCapabilities, displayCapabilities );
        }
    }
    public Vector<ButtonCapabilities> getButtonCapabilities() {
        if (parameters.get(Names.buttonCapabilities) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.buttonCapabilities);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof ButtonCapabilities) {
	                return (Vector<ButtonCapabilities>) list;
	            } else if (obj instanceof Hashtable) {
	                Vector<ButtonCapabilities> newList = new Vector<ButtonCapabilities>();
	                for (Object hashObj : list) {
	                    newList.add(new ButtonCapabilities((Hashtable)hashObj));
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
    public void setButtonCapabilities( Vector<ButtonCapabilities> buttonCapabilities ) {
        if (buttonCapabilities != null) {
            parameters.put(Names.buttonCapabilities, buttonCapabilities );
        }
    }
    public Vector<HmiZoneCapabilities> getHmiZoneCapabilities() {
        if (parameters.get(Names.hmiZoneCapabilities) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.hmiZoneCapabilities);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof HmiZoneCapabilities) {
	                return (Vector<HmiZoneCapabilities>) list;
	            } else if (obj instanceof String) {
	                Vector<HmiZoneCapabilities> newList = new Vector<HmiZoneCapabilities>();
	                for (Object hashObj : list) {
	                    String strFormat = (String)hashObj;
	                    HmiZoneCapabilities toAdd = null;
	                    try {
	                        toAdd = HmiZoneCapabilities.valueForString(strFormat);
	                    } catch (Exception e) {
	                    	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.hmiZoneCapabilities, e);
	                    }
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
    public void setHmiZoneCapabilities( Vector<HmiZoneCapabilities> hmiZoneCapabilities ) {
        if (hmiZoneCapabilities != null) {
            parameters.put(Names.hmiZoneCapabilities, hmiZoneCapabilities );
        }
    }
    public Vector<SpeechCapabilities> getSpeechCapabilities() {
        if (parameters.get(Names.speechCapabilities) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.speechCapabilities);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof SpeechCapabilities) {
	                return (Vector<SpeechCapabilities>) list;
	            } else if (obj instanceof String) {
	                Vector<SpeechCapabilities> newList = new Vector<SpeechCapabilities>();
	                for (Object hashObj : list) {
	                    String strFormat = (String)hashObj;
	                    SpeechCapabilities toAdd = null;
	                    try {
	                        toAdd = SpeechCapabilities.valueForString(strFormat);
	                    } catch (Exception e) {
	                    	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.speechCapabilities, e);
	                    }
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
    public void setSpeechCapabilities( Vector<SpeechCapabilities> speechCapabilities ) {
        if (speechCapabilities != null) {
            parameters.put(Names.speechCapabilities, speechCapabilities );
        }
    }
    public Vector<VrCapabilities> getVrCapabilities() {
        if (parameters.get(Names.vrCapabilities) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.vrCapabilities);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof VrCapabilities) {
	                return (Vector<VrCapabilities>) list;
	            } else if (obj instanceof String) {
	                Vector<VrCapabilities> newList = new Vector<VrCapabilities>();
	                for (Object hashObj : list) {
	                    String strFormat = (String)hashObj;
	                    VrCapabilities toAdd = null;
	                    try {
	                        toAdd = VrCapabilities.valueForString(strFormat);
	                    } catch (Exception e) {
	                    	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.vrCapabilities, e);
	                    }
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
    public void setVrCapabilities( Vector<VrCapabilities> vrCapabilities ) {
        if (vrCapabilities != null) {
            parameters.put(Names.vrCapabilities, vrCapabilities );
        }
    }
}
