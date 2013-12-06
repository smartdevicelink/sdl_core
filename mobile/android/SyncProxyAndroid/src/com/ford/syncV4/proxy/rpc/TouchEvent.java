package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.util.DebugTool;

import java.util.Hashtable;
import java.util.Vector;

public class TouchEvent extends RPCStruct {
    public TouchEvent() {}
    
    public TouchEvent(Hashtable hash) {
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
    
    public void setTs(Vector<Integer> ts) {
        if (ts != null) {
            store.put(Names.ts, ts);
        } else {
        	store.remove(Names.ts);
        }
    }
    
    public Vector<Integer> getTs() {
        Object objList = store.get(Names.ts);
        if (objList instanceof Vector<?>) {
            Vector<?> list = (Vector<?>)objList;
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof Integer) {
                    return (Vector<Integer>)list;
                }
            }
        }
        return null;
    }

    public void setC(Vector<TouchCoord> c) {
        if (c != null) {
            store.put(Names.c, c);
        } else {
        	store.remove(Names.c);
        }
    }

    public Vector<TouchCoord> getC() {
        Object objList = store.get(Names.c);
        if (objList instanceof Vector<?>) {
            Vector<?> list = (Vector<?>)objList;
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof TouchCoord) {
                    return (Vector<TouchCoord>)list;
                }
            }
        }
        return null;
    }
}