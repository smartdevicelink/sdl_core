package com.ford.syncV4.proxy;

import java.util.Hashtable;

import org.json.JSONException;
import org.json.JSONObject;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.constants.Names;

public class RPCStruct {
	
	private byte[] _bulkData = null;

	protected Hashtable<String, Object> store = null;
	
	public RPCStruct() {
		store = new Hashtable<String, Object>();
	}
	
	protected RPCStruct(RPCStruct rpcs) {
		this.store = rpcs.store;
	}
	
	public RPCStruct(Hashtable<String, Object> hashtable) {
		store = hashtable;
		//store = (Hashtable<String, Object>) ObjectCopier.copy(hashtable);
	}
	
	public void deserializeJSON(JSONObject jsonObject) throws JSONException {
		store = JsonRPCMarshaller.deserializeJSONObject(jsonObject);
	}
	
	// deserializeJSONObject method moved to JsonRPCMarshaller for consistency
	// Keep reference here for backwards compatibility
	@Deprecated
	public static Hashtable<String, Object> deserializeJSONObject(JSONObject jsonObject) 
			throws JSONException {
		return JsonRPCMarshaller.deserializeJSONObject(jsonObject);
	}
	
	public JSONObject serializeJSON() throws JSONException {
		return JsonRPCMarshaller.serializeHashtable(store);
	}
	
	public JSONObject serializeJSON(byte version) throws JSONException {
		if (version == 2) {
			String messageType = (String)store.keys().nextElement();
			Hashtable function = (Hashtable)store.get(messageType);
			Hashtable parameters = (Hashtable)function.get(Names.parameters);
			return JsonRPCMarshaller.serializeHashtable(parameters);
		} else return JsonRPCMarshaller.serializeHashtable(store);
	}

	public byte[] getBulkData() {
		return this._bulkData;
	}

	public void setBulkData(byte[] bulkData) {
		if (bulkData != null) {
			this._bulkData = bulkData;
		}
	}
}
