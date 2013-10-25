package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class SetGlobalProperties extends RPCRequest {

    public SetGlobalProperties() {
        super("SetGlobalProperties");
    }
    public SetGlobalProperties(Hashtable hash) {
        super(hash);
    }
    public Vector<TTSChunk> getHelpPrompt() {
    	if (parameters.get(Names.helpPrompt) instanceof Vector<?>) {
	        Vector<?> list = (Vector<?>)parameters.get(Names.helpPrompt);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof TTSChunk) {
	                return (Vector<TTSChunk>) list;
	            } else if (obj instanceof Hashtable) {
	                Vector<TTSChunk> newList = new Vector<TTSChunk>();
	                for (Object hashObj : list) {
	                    newList.add(new TTSChunk((Hashtable)hashObj));
	                }
	                return newList;
	            }
	        }
    	}
	    return null;
    }
    public void setHelpPrompt(Vector<TTSChunk> helpPrompt) {
        if (helpPrompt != null) {
            parameters.put(Names.helpPrompt, helpPrompt);
        }
    }
    public Vector<TTSChunk> getTimeoutPrompt() {
        if (parameters.get(Names.timeoutPrompt) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.timeoutPrompt);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof TTSChunk) {
	                return (Vector<TTSChunk>) list;
	            } else if (obj instanceof Hashtable) {
	                Vector<TTSChunk> newList = new Vector<TTSChunk>();
	                for (Object hashObj : list) {
	                    newList.add(new TTSChunk((Hashtable)hashObj));
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
    public void setTimeoutPrompt(Vector<TTSChunk> timeoutPrompt) {
        if (timeoutPrompt != null) {
            parameters.put(Names.timeoutPrompt, timeoutPrompt);
        }
    }
    public String getVrHelpTitle() {
        return (String) parameters.get(Names.vrHelpTitle);
    }
    public void setVrHelpTitle(String vrHelpTitle) {
        if (vrHelpTitle != null) {
            parameters.put(Names.vrHelpTitle, vrHelpTitle);
        } else {
        	parameters.remove(Names.vrHelpTitle);
        }
    }
    public Vector<VrHelpItem> getVrHelp() {
        if (parameters.get(Names.vrHelp) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.vrHelp);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof VrHelpItem) {
	                return (Vector<VrHelpItem>) list;
	            } else if (obj instanceof Hashtable) {
	                Vector<VrHelpItem> newList = new Vector<VrHelpItem>();
	                for (Object hashObj : list) {
	                    newList.add(new VrHelpItem((Hashtable)hashObj));
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
    public void setVrHelp(Vector<VrHelpItem> vrHelp) {
        if (vrHelp != null) {
            parameters.put(Names.vrHelp, vrHelp);
        } else {
        	parameters.remove(Names.vrHelp);
        }
    }
    public String getMenuTitle() {
        final Object obj = parameters.get(Names.menuTitle);
        if (obj instanceof String) {
            return (String) obj;
        }
        return null;
    }
    public void setMenuTitle(String menuTitle) {
        if (menuTitle != null) {
            parameters.put(Names.menuTitle, menuTitle);
        } else {
            parameters.remove(Names.menuTitle);
        }
    }
    public Image getMenuIcon() {
        final Object obj = parameters.get(Names.menuIcon);
        if (obj instanceof Image) {
            return (Image) obj;
        }
        return null;
    }
    public void setMenuIcon(Image menuIcon) {
        if (menuIcon != null) {
            parameters.put(Names.menuIcon, menuIcon);
        } else {
            parameters.remove(Names.menuIcon);
        }
    }
    public KeyboardProperties getKeyboardProperties() {
        final Object obj = parameters.get(Names.keyboardProperties);
        if (obj instanceof KeyboardProperties) {
            return (KeyboardProperties) obj;
        }
        return null;
    }
    public void setKeyboardProperties(KeyboardProperties keyboardProperties) {
        if (keyboardProperties != null) {
            parameters.put(Names.keyboardProperties, keyboardProperties);
        } else {
            parameters.remove(Names.keyboardProperties);
        }
    }
}
