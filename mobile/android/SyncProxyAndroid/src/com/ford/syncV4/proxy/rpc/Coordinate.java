package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;

import java.util.Hashtable;

public class Coordinate extends RPCStruct {
    public Coordinate() {}
    
    public Coordinate(Hashtable hash) {
        super(hash);
    }
    
    public void setXCoord(Integer xCoord) {
        if (xCoord != null) {
            store.put(Names.xCoord, xCoord);
        } else {
        	store.remove(Names.xCoord);
        }
    }
    
    public Integer getXCoord() {
        return (Integer) store.get(Names.xCoord);
    }
    
    public void setYCoord(Integer yCoord) {
        if (yCoord != null) {
            store.put(Names.yCoord, yCoord);
        } else {
        	store.remove(Names.yCoord);
        }
    }
    
    public Integer getYCoord() {
        return (Integer) store.get(Names.yCoord);
    }
    
}