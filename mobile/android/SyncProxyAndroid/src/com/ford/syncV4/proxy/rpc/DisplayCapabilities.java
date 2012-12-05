package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.DisplayType;
import com.ford.syncV4.proxy.rpc.enums.MediaClockFormat;
import com.ford.syncV4.util.DebugTool;

public class DisplayCapabilities extends RPCStruct {

    public DisplayCapabilities() { }
    public DisplayCapabilities(Hashtable hash) {
        super(hash);
    }
    public DisplayType getDisplayType() {
        Object obj = store.get(Names.displayType);
        if (obj instanceof DisplayType) {
            return (DisplayType) obj;
        } else if (obj instanceof String) {
            DisplayType theCode = null;
            try {
                theCode = DisplayType.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.displayType, e);
            }
            return theCode;
        }
        return null;
    }
    public void setDisplayType( DisplayType displayType ) {
        if (displayType != null) {
            store.put(Names.displayType, displayType );
        }
    }
    public Vector<TextField> getTextFields() {
        if (store.get(Names.textFields) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)store.get(Names.textFields);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof TextField) {
	                return (Vector<TextField>) list;
	            } else if (obj instanceof Hashtable) {
	                Vector<TextField> newList = new Vector<TextField>();
	                for (Object hashObj : list) {
	                    newList.add(new TextField((Hashtable)hashObj));
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
    public void setTextFields( Vector<TextField> textFields ) {
        if (textFields != null) {
            store.put(Names.textFields, textFields );
        }
    }
    public Vector<MediaClockFormat> getMediaClockFormats() {
        if (store.get(Names.mediaClockFormats) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)store.get(Names.mediaClockFormats);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof MediaClockFormat) {
	                return (Vector<MediaClockFormat>) list;
	            } else if (obj instanceof String) {
	                Vector<MediaClockFormat> newList = new Vector<MediaClockFormat>();
	                for (Object hashObj : list) {
	                    String strFormat = (String)hashObj;
	                    MediaClockFormat toAdd = null;
	                    try {
	                        toAdd = MediaClockFormat.valueForString(strFormat);
	                    } catch (Exception e) {
	                        DebugTool.logError("Failed to parse MediaClockFormat from " + getClass().getSimpleName() + "." + Names.mediaClockFormats, e);
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
    public void setMediaClockFormats( Vector<MediaClockFormat> mediaClockFormats ) {
        if (mediaClockFormats != null) {
            store.put(Names.mediaClockFormats, mediaClockFormats );
        }
    }
}
