package com.ford.syncV4.marshal;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCMessage;

public interface IJsonRPCMarshaller {
	public byte[] marshall(RPCMessage msg, byte version);
	public Hashtable<String, Object> unmarshall(byte[] message);
}