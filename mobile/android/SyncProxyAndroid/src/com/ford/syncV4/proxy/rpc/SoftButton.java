package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.SoftButtonType;
import com.ford.syncV4.proxy.rpc.enums.SystemAction;
import com.ford.syncV4.util.DebugTool;

public class SoftButton extends RPCStruct {

    public SoftButton() { }
    public SoftButton(Hashtable hash) {
        super(hash);
    }
    public void setType(SoftButtonType type) {
        if (type != null) {
            store.put(Names.type, type);
        } else {
        	store.remove(Names.type);
        }
    }
    public SoftButtonType getType() {
    	Object obj = store.get(Names.type);
        if (obj instanceof SoftButtonType) {
            return (SoftButtonType) obj;
        } else if (obj instanceof String) {
        	SoftButtonType theCode = null;
            try {
                theCode = SoftButtonType.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.type, e);
            }
            return theCode;
        }
        return null;
    }
    public void setText(String text) {
        if (text != null) {
            store.put(Names.text, text);
        } else {
        	store.remove(Names.text);
        }
    }
    public String getText() {
        return (String) store.get(Names.text);
    }
    public void setImage(Image image) {
        if (image != null) {
            store.put(Names.image, image);
        } else {
        	store.remove(Names.image);
        }
    }
    public Image getImage() {
    	Object obj = store.get(Names.image);
        if (obj instanceof Image) {
            return (Image) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new Image((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.image, e);
            }
        }
        return null;
    }
    public void setIsHighlighted(Boolean isHighlighted) {
        if (isHighlighted != null) {
            store.put(Names.isHighlighted, isHighlighted);
        } else {
        	store.remove(Names.isHighlighted);
        }
    }
    public Boolean getIsHighlighted() {
        return (Boolean) store.get(Names.isHighlighted);
    }
    public void setSoftButtonID(Integer softButtonID) {
        if (softButtonID != null) {
            store.put(Names.softButtonID, softButtonID);
        } else {
        	store.remove(Names.softButtonID);
        }
    }
    public Integer getSoftButtonID() {
        return (Integer) store.get(Names.softButtonID);
    }
    public void setSystemAction(SystemAction systemAction) {
        if (systemAction != null) {
            store.put(Names.systemAction, systemAction);
        } else {
        	store.remove(Names.systemAction);
        }
    }
    public SystemAction getSystemAction() {
    	Object obj = store.get(Names.systemAction);
        if (obj instanceof SystemAction) {
            return (SystemAction) obj;
        } else if (obj instanceof String) {
        	SystemAction theCode = null;
            try {
                theCode = SystemAction.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.systemAction, e);
            }
            return theCode;
        }
        return null;
    }
}
