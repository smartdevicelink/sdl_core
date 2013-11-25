package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.util.DebugTool;

import java.util.Hashtable;

public class TouchEventInfo extends RPCStruct {
    public TouchEventInfo() {}
    
    public TouchEventInfo(Hashtable hash) {
        super(hash);
    }
    
    public void setId(Integer id) {
        if (id != null) {
            store.put(Names.id, id);
        } else {
        	store.remove(Names.id);
        }
    }
    
    public Integer getId() {
        return (Integer) store.get(Names.id);
    }
    
    public void setPoint(Coordinate point) {
        if (point != null) {
            store.put(Names.point, point);
        } else {
        	store.remove(Names.point);
        }
    }
    
    public Coordinate getPoint() {
    	Object obj = store.get(Names.point);
        if (obj instanceof Coordinate) {
            return (Coordinate) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new Coordinate((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.point, e);
            }
        }
        return null;
    }
    
    public void setArea(TouchArea area) {
        if (area != null) {
            store.put(Names.area, area);
        } else {
        	store.remove(Names.area);
        }
    }
    
    public TouchArea getArea() {
    	Object obj = store.get(Names.area);
        if (obj instanceof TouchArea) {
            return (TouchArea) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new TouchArea((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.area, e);
            }
        }
        return null;
    }
}