package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.KeyboardEvent;
import com.ford.syncV4.util.DebugTool;

import java.util.Hashtable;

/**
 * Created by Andrew Batutin on 9/22/13.
 */
public class OnKeyboardInput extends RPCNotification {

    public OnKeyboardInput() {
        super("OnKeyboardInput");
    }

    public OnKeyboardInput(Hashtable hash) {
        super(hash);
    }

    public KeyboardEvent getEvent() {
        Object obj = parameters.get(Names.keyboardEvent);
        if (obj instanceof KeyboardEvent) {
            return (KeyboardEvent) obj;
        } else if (obj instanceof String) {
            KeyboardEvent theCode = null;
            try {
                theCode = KeyboardEvent.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.keyboardEvent, e);
            }
            return theCode;
        }
        return null;
    }

    public void setEvent(KeyboardEvent event) {
        if (event != null) {
            parameters.put(Names.keyboardEvent, event);
        } else {
            parameters.remove(Names.keyboardEvent);
        }
    }

    public void setData(String data) {
        if (data != null) {
            parameters.put(Names.data, data);
        } else {
            parameters.remove(Names.data);
        }
    }
    public String getData() {
        Object obj = parameters.get(Names.data);
        if (obj instanceof String) {
            return (String) obj;
        }
        return null;
    }

    @Override
    public String toString(){
        String result =  this.getFunctionName() +": " + " data: " + this.getData() + " event:" + this.getEvent().toString();
        return result;
    }

}
