package com.ford.syncV4.android.marshaller;

import java.util.Hashtable;

import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.proxy.RPCMessage;

/**
 * A special-case marshaller that will return the same stubbed bytes to respond
 * to any incoming message. Note that you shouldn't use the unmarshalling here
 * (for that case, it throws a RuntimeException).
 */
public class CustomJsonRPCMarshaller implements IJsonRPCMarshaller {
	/** The string that the marshaller will return for any incoming message. */
	private String stubbedValue = null;

	public CustomJsonRPCMarshaller(String stubbedValue) {
		this.stubbedValue = stubbedValue;
	}

	@Override
	public byte[] marshall(RPCMessage msg, byte version) {
		return stubbedValue.getBytes();
	}

	@Override
	public Hashtable<String, Object> unmarshall(byte[] message) {
		throw new RuntimeException(
				"Custom JSON marshaller should only be used to marshall messages");
	}

	public String getStubbedValue() {
		return stubbedValue;
	}

	public void setStubbedValue(String stubbedValue) {
		this.stubbedValue = stubbedValue;
	}
}
