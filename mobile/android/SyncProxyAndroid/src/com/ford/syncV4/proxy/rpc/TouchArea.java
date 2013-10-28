package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.util.DebugTool;

import java.util.Hashtable;

public class TouchArea extends RPCStruct {
    public TouchArea() {}
    
    public TouchArea(Hashtable hash) {
        super(hash);
    }
    
    public void setRotationAngle(Double rotationAngle) {
        if (rotationAngle != null) {
            store.put(Names.rotationAngle, rotationAngle);
        } else {
        	store.remove(rotationAngle);
        }
    }
    
    public Double getRotationAngle() {
        return (Double) store.get(Names.rotationAngle);
    }
    
    public void setRadiusCoord(Coordinate radiusCoord) {
        if (radiusCoord != null) {
            store.put(Names.radiusCoord, radiusCoord);
        } else {
        	store.remove(Names.radiusCoord);
        }
    }
    
    public Coordinate getRadiusCoord() {
    	Object obj = store.get(Names.radiusCoord);
        if (obj instanceof Coordinate) {
            return (Coordinate) obj;
        } else if (obj instanceof Hashtable) {
        	try {
        		return new Coordinate((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.radiusCoord, e);
            }
        }
        return null;
    }
    
}