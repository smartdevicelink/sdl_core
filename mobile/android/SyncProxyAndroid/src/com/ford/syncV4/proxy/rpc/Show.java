package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.TextAlignment;
import com.ford.syncV4.util.DebugTool;

public class Show extends RPCRequest {

    public Show() {
        super("Show");
    }
    public Show(Hashtable hash) {
        super(hash);
    }
    public String getMainField1() {
        return (String) parameters.get(Names.mainField1);
    }
    public void setMainField1(String mainField1) {
        if (mainField1 != null) {
            parameters.put(Names.mainField1, mainField1);
        } else {
        	parameters.remove(Names.mainField1);
        }
    }
    public String getMainField2() {
        return (String) parameters.get(Names.mainField2);
    }
    public void setMainField2(String mainField2) {
        if (mainField2 != null) {
            parameters.put(Names.mainField2, mainField2);
        } else {
        	parameters.remove(Names.mainField2);
        }
    }
    public String getMainField3() {
        return (String) parameters.get(Names.mainField3);
    }
    public void setMainField3(String mainField3) {
        if (mainField3 != null) {
            parameters.put(Names.mainField3, mainField3);
        } else {
        	parameters.remove(Names.mainField3);
        }
    }
    public String getMainField4() {
        return (String) parameters.get(Names.mainField4);
    }
    public void setMainField4(String mainField4) {
        if (mainField4 != null) {
            parameters.put(Names.mainField4, mainField4);
        } else {
        	parameters.remove(Names.mainField4);
        }
    }
    public TextAlignment getAlignment() {
        Object obj = parameters.get(Names.alignment);
        if (obj instanceof TextAlignment) {
            return (TextAlignment) obj;
        } else if (obj instanceof String) {
            TextAlignment theCode = null;
            try {
                theCode = TextAlignment.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.alignment, e);
            }
            return theCode;
        }
        return null;
    }
    public void setAlignment(TextAlignment alignment) {
        if (alignment != null) {
            parameters.put(Names.alignment, alignment);
        } else {
        	parameters.remove(Names.alignment);
        }
    }
    public String getStatusBar() {
        return (String) parameters.get(Names.statusBar);
    }
    public void setStatusBar(String statusBar) {
        if (statusBar != null) {
            parameters.put(Names.statusBar, statusBar);
        } else {
        	parameters.remove(Names.statusBar);
        }
    }
    public String getMediaClock() {
        return (String) parameters.get(Names.mediaClock);
    }
    public void setMediaClock(String mediaClock) {
        if (mediaClock != null) {
            parameters.put(Names.mediaClock, mediaClock);
        } else {
        	parameters.remove(Names.mediaClock);
        }
    }
    public String getMediaTrack() {
        return (String) parameters.get(Names.mediaTrack);
    }
    public void setMediaTrack(String mediaTrack) {
        if (mediaTrack != null) {
            parameters.put(Names.mediaTrack, mediaTrack);
        } else {
        	parameters.remove(Names.mediaTrack);
        }
    }
    public void setGraphic(Image graphic) {
        if (graphic != null) {
            parameters.put(Names.graphic, graphic);
        } else {
        	parameters.remove(Names.graphic);
        }
    }
    public Image getGraphic() {
    	Object obj = parameters.get(Names.graphic);
        if (obj instanceof Image) {
            return (Image) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new Image((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.graphic, e);
            }
        }
        return null;
    }
    public Vector<SoftButton> getSoftButtons() {
        if (parameters.get(Names.softButtons) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.softButtons);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof SoftButton) {
	                return (Vector<SoftButton>) list;
	            } else if (obj instanceof Hashtable) {
	                Vector<SoftButton> newList = new Vector<SoftButton>();
	                for (Object hashObj : list) {
	                    newList.add(new SoftButton((Hashtable)hashObj));
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
    public void setSoftButtons(Vector<SoftButton> softButtons) {
        if (softButtons != null) {
            parameters.put(Names.softButtons, softButtons);
        } else {
        	parameters.remove(Names.softButtons);
        }
    }
    public Vector<String> getCustomPresets() {
    	if (parameters.get(Names.customPresets) instanceof Vector<?>) {
    		Vector<?> list = (Vector<?>)parameters.get(Names.customPresets);
    		if (list != null && list.size()>0) {
    			Object obj = list.get(0);
    			if (obj instanceof String) {
    				return (Vector<String>) list;
    			}
    		}
    	}
        return null;
    }
    public void setCustomPresets(Vector<String> customPresets) {
        if (customPresets != null) {
            parameters.put(Names.customPresets, customPresets);
        } else {
        	parameters.remove(Names.customPresets);
        }
    }
}
