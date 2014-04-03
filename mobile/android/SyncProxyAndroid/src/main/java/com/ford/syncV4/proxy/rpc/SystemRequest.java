package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.util.DebugTool;

import java.util.Hashtable;

/**
 * An asynchronous request from the device.
 * Binary data can be included in hybrid part of message for some requests
 * (such as HTTP, Proprietary, or Authentication requests)
 */
public class SystemRequest extends RPCRequest {

    public SystemRequest() {
        super(Names.SystemRequest);
    }

    public SystemRequest(Hashtable hash) {
        super(hash);
    }

    public SystemRequest(SystemRequest that) {
        super(that);
    }

    /**
     * Get the type of system request. Note that Proprietary requests should forward the binary
     * data to the known proprietary module on the system.
     * @return {@link com.ford.syncV4.proxy.rpc.enums.RequestType} object
     */
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

    /**
     * Set the type of system request. Note that Proprietary requests should forward the binary
     * data to the known proprietary module on the system.
     * @param requestType {@link com.ford.syncV4.proxy.rpc.enums.RequestType} object
     */
    public void setRequestType(RequestType requestType) {
        if (requestType != null) {
            parameters.put(Names.requestType, requestType);
        } else {
            parameters.remove(Names.requestType);
        }
    }

    /**
     * Filename of {@link com.ford.syncV4.proxy.rpc.enums.RequestType#HTTP} data to store in
     * predefined system staging area. Mandatory if requestType is
     * {@link com.ford.syncV4.proxy.rpc.enums.RequestType#HTTP}.
     * {@link com.ford.syncV4.proxy.rpc.enums.RequestType#PROPRIETARY}
     * requestType should ignore this parameter.
     * @return file name
     */
    public String getFileName() {
        final Object object = parameters.get(Names.fileName);
        if (object instanceof String) {
            return (String) object;
        }
        return null;
    }

    /**
     * Filename of {@link com.ford.syncV4.proxy.rpc.enums.RequestType#HTTP} data to store in
     * predefined system staging area. Mandatory if requestType is
     * {@link com.ford.syncV4.proxy.rpc.enums.RequestType#HTTP}.
     * {@link com.ford.syncV4.proxy.rpc.enums.RequestType#PROPRIETARY}
     * requestType should ignore this parameter.
     * @param fileName file name
     */
    public void setFileName(String fileName) {
        if (fileName != null) {
            parameters.put(Names.fileName, fileName);
        } else {
            parameters.remove(Names.fileName);
        }
    }
}