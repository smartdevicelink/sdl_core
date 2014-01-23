package com.ford.syncV4.proxy;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Set;

import com.ford.syncV4.proxy.constants.Names;

public class RPCMessage extends RPCStruct  {

	public RPCMessage(String functionName) {
		this(functionName, "request");
	}
	
	protected RPCMessage(RPCMessage rpcm) {
//		this(rpcm.store);
        // FIXME test this!
        this.store = new Hashtable<String, Object>();
        this.messageType = rpcm.messageType;
        this.function = (Hashtable) rpcm.function.clone();
        this.store.put(messageType, function);
        this.parameters = (Hashtable) rpcm.parameters.clone();
        this.function.put(Names.parameters, this.parameters);
        this.function.put(Names.function_name, rpcm.getFunctionName());
	}
	
	protected RPCMessage(RPCStruct rpcs) {
		this("", "");
		this.parameters = rpcs.store;
	}
	
	public RPCMessage(String functionName, String messageType) {
		function = new Hashtable();
		this.messageType = messageType;
		store.put(messageType, function);
		parameters = new Hashtable();
		function.put(Names.parameters, parameters);
		function.put(Names.function_name, functionName);
	}

    public RPCMessage(Hashtable hash) {
        store = hash;
        messageType = getMessageTypeName(hash.keySet());

        function = (Hashtable)hash.get(messageType);
        parameters = (Hashtable)function.get(Names.parameters);
    }

	protected String messageType;
	protected Hashtable parameters;
	protected Hashtable function;
	
	public String getFunctionName() {
		return (String)function.get(Names.function_name);
	}
	
	protected void setFunctionName(String functionName) {
		function.put(Names.function_name, functionName);
	}

	public String getMessageType() {
		return messageType;
	}
	
	public void setParameters(String functionName, Object value) {
		if (value != null) {
			parameters.put(functionName, value);
		} else {
			parameters.remove(functionName);
		}
	}
	
	public Object getParameters(String functionName) {
		return parameters.get(functionName);
	}
}