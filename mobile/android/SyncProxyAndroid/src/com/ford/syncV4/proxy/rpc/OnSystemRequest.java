package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.util.DebugTool;

import java.util.Hashtable;
import java.util.Vector;

public class OnSystemRequest extends RPCNotification {

    public OnSystemRequest() {
        super("OnSystemRequest");
    }

    public OnSystemRequest(Hashtable hash) {
        super(hash);
    }

    public RequestType getRequestType() {
        Object obj = parameters.get(Names.requestType);
        if (obj instanceof RequestType) {
            return (RequestType) obj;
        } else if (obj instanceof String) {
            RequestType theCode = null;
            try {
                theCode = RequestType.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.requestType, e);
            }
            return theCode;
        }
        return null;
    }

    public void setRequestType(RequestType requestType) {
        if (requestType != null) {
            parameters.put(Names.requestType, requestType);
        } else {
            parameters.remove(Names.requestType);
        }
    }

    public Vector<String> getUrl() {
        if (parameters.get(Names.url) instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) parameters.get(Names.url);
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof String) {
                    return (Vector<String>) list;
                }
            }
        }
        return null;
    }

    public void setUrl(Vector<String> url) {
        if (url != null) {
            parameters.put(Names.url, url);
        } else {
            parameters.remove(Names.url);
        }
    }

    public FileType getFileType() {
        Object obj = parameters.get(Names.fileType);
        if (obj instanceof FileType) {
            return (FileType) obj;
        } else if (obj instanceof String) {
            FileType theCode = null;
            try {
                theCode = FileType.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.fileType, e);
            }
            return theCode;
        }
        return null;
    }

    public void setFileType(FileType fileType) {
        if (fileType != null) {
            parameters.put(Names.fileType, fileType);
        } else {
            parameters.remove(Names.fileType);
        }
    }

    public Integer getOffset() {
        final Object o = parameters.get(Names.offset);
        if (o instanceof Integer) {
            return (Integer) o;
        }
        return null;
    }

    public void setOffset(Integer offset) {
        if (offset != null) {
            parameters.put(Names.offset, offset);
        } else {
            parameters.remove(Names.offset);
        }
    }

    public Integer getLength() {
        final Object o = parameters.get(Names.length);
        if (o instanceof Integer) {
            return (Integer) o;
        }
        return null;
    }

    public void setLength(Integer length) {
        if (length != null) {
            parameters.put(Names.length, length);
        } else {
            parameters.remove(Names.length);
        }
    }

    public Integer getTimeout(){
        if (parameters.get(Names.timeout) instanceof Integer) {
            return (Integer)parameters.get(Names.timeout);
        }
        return null;
    }
    public void setTimeout(Integer timeout) {
        if (timeout != null) {
            parameters.put(Names.timeout, timeout);
        } else {
            parameters.remove(Names.timeout);
        }
    }
}
