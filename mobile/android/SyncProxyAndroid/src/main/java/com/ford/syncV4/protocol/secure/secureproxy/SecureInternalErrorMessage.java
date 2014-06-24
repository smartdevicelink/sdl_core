package com.ford.syncV4.protocol.secure.secureproxy;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

/**
 * Created by Andrew Batutin on 6/23/14.
 */
public class SecureInternalErrorMessage extends RPCRequest {


    public SecureInternalErrorMessage() {
        super(Names.internalError);
    }

    public void setErrorId(int id){
        parameters.put(Names.errorId, id);
    }

    public int getErrorId() {
        return (int) parameters.get(Names.errorId);
    }

    public void setMessage(String message) {
        parameters.put(Names.errorMessage, message);
    }

    public String getMessage() {
        return (String) parameters.get(Names.errorMessage);
    }

    public void setMessageType(String messageType){
        this.messageType = messageType;
    }

}
