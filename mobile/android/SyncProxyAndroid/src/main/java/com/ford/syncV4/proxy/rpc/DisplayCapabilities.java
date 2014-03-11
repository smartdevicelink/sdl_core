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

    public Vector<ImageField> getImageFields() {
        if (store.get(Names.imageFields) instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) store.get(Names.imageFields);
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof ImageField) {
                    return (Vector<ImageField>) list;
                } else if (obj instanceof Hashtable) {
                    Vector<ImageField> newList = new Vector<ImageField>();
                    for (Object hashObj : list) {
                        newList.add(new ImageField((Hashtable) hashObj));
                    }
                    return newList;
                }
            }
        }
        return null;
    }

    public void setImageFields(Vector<ImageField> imageFields) {
        if (imageFields != null) {
            store.put(Names.imageFields, imageFields);
        } else {
            store.remove(Names.imageFields);
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

    public void setGraphicSupported(Boolean graphicSupported) {
        if (graphicSupported != null) {
            store.put(Names.graphicSupported, graphicSupported);
        } else {
            store.remove(Names.graphicSupported);
        }
    }

    public Boolean getGraphicSupported() {
        final Object o = store.get(Names.graphicSupported);
        if (o instanceof Boolean) {
            return (Boolean) o;
        }
        return null;
    }

    public Vector<String> getTemplatesAvailable() {
        if (store.get(Names.templatesAvailable) instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) store.get(Names.templatesAvailable);
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof String) {
                    return (Vector<String>) list;
                }
            }
        }
        return null;
    }

    public void setTemplatesAvailable(Vector<String> templatesAvailable) {
        if (templatesAvailable != null) {
            store.put(Names.templatesAvailable, templatesAvailable);
        } else {
            store.remove(Names.templatesAvailable);
        }
    }

    public ScreenParams getScreenParams() {
        Object obj = store.get(Names.screenParams);
        if (obj instanceof ScreenParams) {
            return (ScreenParams) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new ScreenParams((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.screenParams, e);
            }
        }
        return null;
    }

    public void setScreenParams(ScreenParams screenParams) {
        if (screenParams != null) {
            store.put(Names.screenParams, screenParams);
        } else {
            store.remove(Names.screenParams);
        }
    }

    public void setNumCustomPresetsAvailable(
            Integer numCustomPresetsAvailable) {
        if (numCustomPresetsAvailable != null) {
            store.put(Names.numCustomPresetsAvailable,
                    numCustomPresetsAvailable);
        } else {
            store.remove(Names.numCustomPresetsAvailable);
        }
    }

    public Integer getNumCustomPresetsAvailable() {
        final Object o = store.get(Names.numCustomPresetsAvailable);
        if (o instanceof Integer) {
            return (Integer) o;
        }
        return null;
    }
}
