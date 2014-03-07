package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.SystemAction;
import com.ford.syncV4.util.DebugTool;

public class ScrollableMessage extends RPCRequest {

    public ScrollableMessage() {
        super("ScrollableMessage");
    }
    public ScrollableMessage(Hashtable hash) {
        super(hash);
    }
    public void setScrollableMessageBody(String scrollableMessageBody) {
        if (scrollableMessageBody != null) {
            parameters.put(Names.scrollableMessageBody, scrollableMessageBody);
        } else {
        	parameters.remove(Names.scrollableMessageBody);
        }
    }
    public String getScrollableMessageBody() {
        return (String) parameters.get(Names.scrollableMessageBody);
    }
    public void setTimeout(Integer timeout) {
        if (timeout != null) {
            parameters.put(Names.timeout, timeout);
        } else {
        	parameters.remove(Names.timeout);
        }
    }
    public Integer getTimeout() {
        return (Integer) parameters.get(Names.timeout);
    }
    public void setSoftButtons(Vector<SoftButton> softButtons) {
        if (softButtons != null) {
            parameters.put(Names.softButtons, softButtons);
        } else {
        	parameters.remove(Names.softButtons);
        }
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
	                    newList.add(new SoftButton((Hashtable) hashObj));
	                }
	                return newList;
	            }
	        }
        }
        return null;
    }
}
