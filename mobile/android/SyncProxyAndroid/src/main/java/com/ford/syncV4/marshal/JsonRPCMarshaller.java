package com.ford.syncV4.marshal;

import com.ford.syncV4.proxy.RPCMessage;
import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.util.logger.Logger;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.Hashtable;
import java.util.Iterator;
import java.util.Vector;

/*
 * Responsible for marshalling and unmarshing between RPC Objects and byte streams that are sent
 * over transmission
 */
public class JsonRPCMarshaller implements IJsonRPCMarshaller {

    private static final String CLASS_NAME = JsonRPCMarshaller.class.getSimpleName();

    public byte[] marshall(RPCMessage msg, byte version) {
        byte[] jsonBytes = null;
        try {
            JSONObject jsonObject = msg.serializeJSON(version);
            jsonBytes = jsonObject.toString().getBytes();

            Logger.d(CLASS_NAME + " transmit " + jsonObject);
        } catch (JSONException e) {
            Logger.e("Failed to encode messages to JSON.", e);
        }
        return jsonBytes;
    }

    public Hashtable<String, Object> unmarshall(byte[] message) {
        if (message != null) {
            Logger.d(CLASS_NAME + " receive " + message.length + " bytes");
        } else {
            Logger.w(CLASS_NAME + " receive null bytes");
        }
        Hashtable<String, Object> ret = null;
        String jsonString = new String(message);
        try {
            JSONObject jsonObject = new JSONObject(jsonString);
            ret = deserializeJSONObject(jsonObject);
        } catch (JSONException e) {
            Logger.e("Can't parse JSON: " + jsonString, e);
        }
        return ret;
    }

    public static Hashtable<String, Object> deserializeJSONObject(JSONObject jsonObject)
            throws JSONException {
        Hashtable<String, Object> ret = new Hashtable<String, Object>();
        Iterator it = jsonObject.keys();
        String key = null;
        while (it.hasNext()) {
            key = (String) it.next();
            Object value = jsonObject.get(key);
            if (value instanceof JSONObject) {
                ret.put(key, deserializeJSONObject((JSONObject) value));
            } else if (value instanceof JSONArray) {
                JSONArray arrayValue = (JSONArray) value;
                Vector<Object> putList = new Vector<Object>(arrayValue.length());
                for (int i = 0; i < arrayValue.length(); i++) {
                    Object anObject = arrayValue.get(i);
                    if (anObject instanceof JSONObject) {
                        Hashtable<String, Object> deserializedObject = deserializeJSONObject(
                                (JSONObject) anObject);
                        putList.add(deserializedObject);
                    } else {
                        putList.add(anObject);
                    }
                }
                ret.put(key, putList);
            } else {
                ret.put(key, value);
            }
        }
        return ret;
    }

    private static JSONArray serializeVector(Vector vector) throws JSONException {
        JSONArray toPut = new JSONArray();
        for (Object anObject : (vector)) {
            if (anObject instanceof RPCStruct) {
                RPCStruct toSerialize = (RPCStruct) anObject;
                toPut.put(toSerialize.serializeJSON());
            } else if (anObject instanceof Hashtable) {
                Hashtable toSerialize = (Hashtable) anObject;
                toPut.put(serializeHashtable(toSerialize));
            } else {
                toPut.put(anObject);
            }
        }
        return toPut;
    }

    public static JSONObject serializeHashtable(Hashtable<String, Object> hash) throws JSONException {
        JSONObject obj = new JSONObject();
        for (String key : hash.keySet()) {
            Object value = hash.get(key);
            if (value instanceof RPCStruct) {
                obj.put(key, ((RPCStruct) value).serializeJSON());
            } else if (value instanceof Vector<?>) {
                obj.put(key, serializeVector((Vector) value));
            } else if (value instanceof Hashtable) {
                obj.put(key, serializeHashtable((Hashtable) value));
            } else {
                obj.put(key, value);
            }
        }
        return obj;
    }

	/*
	public static JSONObject serializeHashtable(Hashtable<String, Object> hash) 
			throws JSONException {
		JSONObject obj = new JSONObject();
		Iterator<String> hashKeyIterator = hash.keySet().iterator();
		while (hashKeyIterator.hasNext()) {
			String key = (String) hashKeyIterator.next();
			Object value = hash.get(key);
			if (value instanceof RPCStruct) {
				obj.put(key, ((RPCStruct) value).serializeJSON());
			} else if (value instanceof Vector<?>) {
				JSONArray toPut = new JSONArray();
				Iterator<Object> valueIterator = ((Vector) value).iterator();
				while (valueIterator.hasNext()) {
					Object anObject = valueIterator.next();					
					if (anObject instanceof RPCStruct) {
						RPCStruct toSerialize = (RPCStruct) anObject;
						toPut.put(toSerialize.serializeJSON());
					} else if (anObject instanceof Hashtable) {
						Hashtable hashtable = (Hashtable) anObject;
						RPCStruct toSerialize = new RPCStruct(hashtable);
						toPut.put(toSerialize.serializeJSON());
					} else {
						toPut.put(anObject);
					}
				}
				obj.put(key, toPut);
			} else if (value instanceof Hashtable) {
				obj.put(key, serializeHashtable((Hashtable)value));
			} else {
				obj.put(key, value);
			}
		}
		return obj;
	}
	*/
}
