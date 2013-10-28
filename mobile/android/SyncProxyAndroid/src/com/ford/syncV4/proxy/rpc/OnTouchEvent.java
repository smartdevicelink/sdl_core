package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.TouchEvent;
import com.ford.syncV4.util.DebugTool;

import java.util.Hashtable;

public class OnTouchEvent extends RPCNotification {
    public OnTouchEvent() {
        super("OnTouchEvent");
    }
    public OnTouchEvent(Hashtable hash) {
        super(hash);
    }
    
    public void setEventType(TouchEvent eventType) {
    	if (eventType != null) {
    		parameters.put(Names.eventType, eventType);
    	} else {
    		parameters.remove(Names.eventType);
    	}
    }
    
    public TouchEvent getEventType() {
        Object obj = parameters.get(Names.eventType);
        if (obj instanceof TouchEvent) {
            return (TouchEvent) obj;
        } else if (obj instanceof String) {
        	TouchEvent theCode = null;
            try {
                theCode = TouchEvent.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.eventType, e);
            }
            return theCode;
        }
        return null;
    }
    
    public void setTouchLists(TouchLists touchLists) {
        if (touchLists != null) {
            parameters.put(Names.touchLists, touchLists);
        } else {
        	parameters.remove(Names.touchLists);
        }
    }
    
    public TouchLists getTouchLists() {
    	Object obj = parameters.get(Names.touchLists);
        if (obj instanceof TouchLists) {
            return (TouchLists) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new TouchLists((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.touchLists, e);
            }
        }
        return null;
    }
    
    public void setInfo(TouchEventInfo info) {
        if (info != null) {
            parameters.put(Names.info, info);
        } else {
        	parameters.remove(Names.info);
        }
    }
    
    public TouchEventInfo getInfo() {
    	Object obj = parameters.get(Names.info);
        if (obj instanceof TouchEventInfo) {
            return (TouchEventInfo) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new TouchEventInfo((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.info, e);
            }
        }
        return null;
    }
}