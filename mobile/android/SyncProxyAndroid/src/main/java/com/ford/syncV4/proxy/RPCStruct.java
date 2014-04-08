package com.ford.syncV4.proxy;

import java.util.Hashtable;
import java.util.Set;

import org.json.JSONException;
import org.json.JSONObject;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.constants.ProtocolConstants;

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
        if (version >= ProtocolConstants.PROTOCOL_VERSION_TWO) {
            String messageType = getMessageTypeName(store.keySet());
            Hashtable function = (Hashtable) store.get(messageType);
            Hashtable parameters = (Hashtable) function.get(Names.parameters);
            return JsonRPCMarshaller.serializeHashtable(parameters);
            //Hashtable hashToSend = new Hashtable();
            //hashToSend.put(Names.parameters, parameters);
            //return JsonRPCMarshaller.serializeHashtable(hashToSend);
        } else {
            return JsonRPCMarshaller.serializeHashtable(store);
        }
    }

    public byte[] getBulkData() {
        return this._bulkData;
    }

    public void setBulkData(byte[] bulkData) {
        if (bulkData != null) {
            this._bulkData = new byte[bulkData.length];
            System.arraycopy(bulkData, 0, _bulkData, 0, bulkData.length);
            //this._bulkData = bulkData;
        }
    }

    /**
     * Workaround method to get RPC message type from a table.
     *
     * @param keys Set of the keys in the Table
     * @return founded value or null
     */
    protected String getMessageTypeName(Set<String> keys) {
        for (String key : keys) {
            if (key == null) {
                continue;
            }
            if (key.equals(Names.request) || key.equals(Names.response) ||
                    key.equals(Names.notification)) {
                return key;
            }
        }
        return null;
    }

    /**
     * Method to check whether provided key contains in the Table
     *
     * @param keys Set of the keys in the Table
     * @return true if found, else - otherwise
     */
    protected boolean hasKey(Set<String> keys, String keyName) {
        for (String key : keys) {
            if (key == null) {
                continue;
            }
            if (key.equals(keyName)) {
                return true;
            }
        }
        return false;
    }
}