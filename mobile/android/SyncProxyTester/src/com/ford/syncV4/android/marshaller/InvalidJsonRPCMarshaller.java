package com.ford.syncV4.android.marshaller;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.RPCMessage;

/**
 * Wraps {@link JsonRPCMarshaller} to produce an invalid marshalled JSON
 * message.
 */
public class InvalidJsonRPCMarshaller extends JsonRPCMarshaller {
	
	private static final String TAG = InvalidJsonRPCMarshaller.class.getSimpleName();

	@Override
	public byte[] marshall(RPCMessage msg, byte version) {
		byte[] msgBytes = super.marshall(msg, version);
		
		if (msgBytes != null) {
			final int newLength = msgBytes.length - 1;
			byte[] bytes = new byte[newLength];
			System.arraycopy(msgBytes, 0, bytes, 0, newLength);
			return bytes;
		} else {
			return null;
		}
	}
}
