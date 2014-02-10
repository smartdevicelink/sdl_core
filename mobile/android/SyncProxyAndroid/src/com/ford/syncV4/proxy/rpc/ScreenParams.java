package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.util.DebugTool;

import java.util.Hashtable;

public class ScreenParams extends RPCStruct {
    public ScreenParams() {
    }

    public ScreenParams(Hashtable hash) {
        super(hash);
    }

    public ImageResolution getResolution() {
        Object obj = store.get(Names.resolution);
        if (obj instanceof ImageResolution) {
            return (ImageResolution) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new ImageResolution((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.resolution, e);
            }
        }
        return null;
    }

    public void setResolution(ImageResolution resolution) {
        if (resolution != null) {
            store.put(Names.resolution, resolution);
        } else {
            store.remove(Names.resolution);
        }
    }

    public TouchEventCapabilities getTouchEventAvailable() {
        Object obj = store.get(Names.touchEventAvailable);
        if (obj instanceof TouchEventCapabilities) {
            return (TouchEventCapabilities) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new TouchEventCapabilities((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.touchEventAvailable, e);
            }
        }
        return null;
    }

    public void setTouchEventAvailable(
            TouchEventCapabilities touchEventCapabilities) {
        if (touchEventCapabilities != null) {
            store.put(Names.touchEventAvailable, touchEventCapabilities);
        } else {
            store.remove(Names.touchEventAvailable);
        }
    }
}