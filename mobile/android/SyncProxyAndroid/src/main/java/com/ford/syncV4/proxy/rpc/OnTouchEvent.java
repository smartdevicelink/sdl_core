package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.TouchType;
import com.ford.syncV4.util.DebugTool;

import java.util.Hashtable;
import java.util.Vector;

public class OnTouchEvent extends RPCNotification {
    public OnTouchEvent() {
        super("OnTouchEvent");
    }
    public OnTouchEvent(Hashtable hash) {
        super(hash);
    }
    
    public void setType(TouchType type) {
    	if (type != null) {
    		parameters.put(Names.type, type);
    	} else {
    		parameters.remove(Names.type);
    	}
    }
    
    public TouchType getType() {
        Object obj = parameters.get(Names.type);
        if (obj instanceof TouchType) {
            return (TouchType) obj;
        } else if (obj instanceof String) {
        	TouchType theCode = null;
            try {
                theCode = TouchType.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.type, e);
            }
            return theCode;
        }
        return null;
    }
    
    public void setEvent(Vector<TouchType> event) {
        if (event != null) {
            parameters.put(Names.event, event);
        } else {
        	parameters.remove(Names.event);
        }
    }
    
    public Vector<TouchType> getEvent() {
        Object objList = parameters.get(Names.event);
        if (objList instanceof Vector<?>) {
            Vector<?> list = (Vector<?>)objList;
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof TouchType) {
                    return (Vector<TouchType>)list;
                }
            }
        }
        return null;
    }
}