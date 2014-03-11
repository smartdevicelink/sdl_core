package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.util.DebugTool;

public class VrHelpItem extends RPCStruct {

    public VrHelpItem() { }
    public VrHelpItem(Hashtable hash) {
        super(hash);
    }
    public void setText(String text) {
        if (text != null) {
            store.put(Names.text, text);
        } else {
        	store.remove(Names.text);
        }
    }
    public String getText() {
        return (String) store.get(Names.menuName);
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
    public void setPosition(Integer position) {
        if (position != null) {
            store.put(Names.position, position);
        } else {
        	store.remove(Names.position);
        }
    }
    public Integer getPosition() {
        return (Integer) store.get(Names.position);
    }
}
