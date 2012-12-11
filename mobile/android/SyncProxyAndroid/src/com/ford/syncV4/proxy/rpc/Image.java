package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.ImageType;
import com.ford.syncV4.util.DebugTool;

public class Image extends RPCStruct {

    public Image() { }
    public Image(Hashtable hash) {
        super(hash);
    }
    public void setValue(String value) {
        if (value != null) {
            store.put(Names.value, value);
        } else {
        	store.remove(Names.value);
        }
    }
    public String getValue() {
        return (String) store.get(Names.value);
    }
    public void setImageType(ImageType imageType) {
        if (imageType != null) {
            store.put(Names.imageType, imageType);
        } else {
        	store.remove(Names.imageType);
        }
    }
    public ImageType getImageType() {
    	Object obj = store.get(Names.imageType);
        if (obj instanceof ImageType) {
            return (ImageType) obj;
        } else if (obj instanceof String) {
        	ImageType theCode = null;
            try {
                theCode = ImageType.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.imageType, e);
            }
            return theCode;
        }
        return null;
    }
}
