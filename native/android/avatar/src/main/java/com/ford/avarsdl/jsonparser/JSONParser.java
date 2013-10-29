package com.ford.avarsdl.jsonparser;

import org.json.JSONException;
import org.json.JSONObject;

import com.ford.avarsdl.util.Const;
import com.ford.avarsdl.util.RPCConst;

import android.util.Log;

public class JSONParser {
	// ===================================================
	// constructors
	// ===================================================
	public JSONParser(String strObj) {
		try {
			mjsonObj = new JSONObject(strObj);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
	};

	public JSONParser(JSONObject obj) {
		this(obj.toString());
	};

	public JSONParser() {
		putEmptyJSONRPCObject();
	};

	// ===================================================
	// Creators
	// ===================================================
	public String createJSONServerRequest(String method, JSONObject params,
			Integer id) {

		try {
			// put method name
			mjsonObj.put(RPCConst.TAG_METHOD, method);
			// put method parameters
			mjsonObj.put(RPCConst.TAG_PARAMETERS, params);
			// put id
			mjsonObj.put(RPCConst.TAG_ID, id);

		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
		return mjsonObj.toString() + System.getProperty("line.separator");
	}

	public String createJSONServerRequest(String method, String params,
			Integer id) {
		try {
			return this.createJSONServerRequest(method, new JSONObject(params),
					id);
		} catch (JSONException e) {
			e.printStackTrace();
			return null;
		}
	}

	public String createJSONResponse(int id, String result) {
		try {
			// look if result contains error
			JSONObject res = new JSONObject(result);
			String errMsg = null;
			try {
				errMsg = res.getString(RPCConst.TAG_ERROR);
			} catch (JSONException e) {/* do nothing */
			}
			if (errMsg == null) {
				JSONObject jsonRes = new JSONObject(result);
				mjsonObj.put(RPCConst.TAG_RESULT, jsonRes);
				mjsonObj.put(RPCConst.TAG_ERROR, null);
			} else {
				JSONObject error = new JSONObject();
				error.put(RPCConst.TAG_ERROR_CODE,
						res.getInt(RPCConst.TAG_ERROR_CODE));
				try {
					error.put(RPCConst.TAG_ERROR_MESSAGE,
							res.getString(RPCConst.TAG_ERROR_MESSAGE));
					error.put(RPCConst.TAG_ERROR_DATA,
							res.getInt(RPCConst.TAG_ERROR_DATA));
				} catch (JSONException e) { /* do nothing */
				}
				mjsonObj.put(RPCConst.TAG_ERROR, error);
				mjsonObj.put(RPCConst.TAG_RESULT, null);
			}
			mjsonObj.put(RPCConst.TAG_ID, id);
			return mjsonObj.toString() + System.getProperty("line.separator");
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
		return null;
	}

	public String createJSONNotification(String method, String params) {

		try {
			// put method name
			mjsonObj.put(RPCConst.TAG_METHOD, method);
			// put method parameters
			mjsonObj.put(RPCConst.TAG_PARAMETERS, new JSONObject(params));// send
																			// only
																			// first
																			// param
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
		return mjsonObj.toString() + System.getProperty("line.separator");
	}

	// ===================================================
	// getters
	// ===================================================

	public String getJSONVersion() {
		try {
			return mjsonObj.getString(RPCConst.TAG_JSONRPC_VERSION);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
		return null;
	}

	public String getMethod() {
		try {
			return mjsonObj.getString(RPCConst.TAG_METHOD);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
		return null;
	}

	public int getId() {
		try {
			return mjsonObj.getInt(RPCConst.TAG_ID);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
		return -1;
	}

	public JSONObject getParams() {
		try {
			return mjsonObj.getJSONObject(RPCConst.TAG_PARAMETERS);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
		return null;
	}

	public String getResult() {
		try {
			return mjsonObj.getString(RPCConst.TAG_RESULT);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
		return null;
	}

	public String getError() {
		JSONObject jsonErr;
		Integer errCode = 0;
		String errMsg = null, errData = null, error = null;
		try {
			jsonErr = mjsonObj.getJSONObject(RPCConst.TAG_ERROR);
			errCode = Integer.valueOf(jsonErr.getInt(RPCConst.TAG_ERROR_CODE));
			errMsg = jsonErr.getString(RPCConst.TAG_ERROR_MESSAGE);
			errData = jsonErr.getString(RPCConst.TAG_ERROR_DATA);
			error = errCode.toString() + " : " + errMsg + " : " + errData;
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
		return error;
	}

	public String getJSONObject() {
		return mjsonObj.toString();
	}

	public String getStringParam(String name) {
		try {
			return mjsonObj.getString(name);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
			return null;
		}
	}

	public int getIntParam(String name) {
		try {
			return mjsonObj.getInt(name);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
			return 0;
		}
	}

	public double getDoubleParam(String name) {
		try {
			return mjsonObj.getDouble(name);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
			return 0;
		}
	}

	public boolean getBooleanParam(String name) {
		try {
			return mjsonObj.getBoolean(name);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
			return false;
		}
	}

	public String getJSONObjectParam(String name) {
		try {
			return mjsonObj.getJSONObject(name).toString();
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
			return null;
		}
	}

	// ===================================================
	// writers
	// ===================================================
	public void putJSONObject(String jsonObj) {
		try {
			mjsonObj = new JSONObject(jsonObj);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
	}

	public void putEmptyJSONRPCObject() {
		mjsonObj = new JSONObject();
		try {
			mjsonObj.put(RPCConst.TAG_JSONRPC_VERSION,
					RPCConst.TAG_JSONRPC_VERSION_VALUE);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
	}

	public void putEmptyJSONObject() {
		mjsonObj = new JSONObject();
	}

	public void putJSONObject(JSONObject obj) {
		mjsonObj = obj;
	}

	public void putStringValue(String sName, String sValue) {
		try {
			mjsonObj.put(sName, sValue);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
	}

	public void putIntValue(String sName, int iValue) {
		try {
			mjsonObj.put(sName, iValue);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
	}

	public void putBooleanValue(String sName, boolean bValue) {
		try {
			mjsonObj.put(sName, bValue);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
	}

	public void putDoubleValue(String sName, double dValue) {
		try {
			mjsonObj.put(sName, dValue);
		} catch (JSONException e) {
			logMsg('w', e.getMessage());
		}
	}

	// ================================================================
	// private section
	// ================================================================
	private final boolean DEBUG = false;
	private final String TAG_NAME = "JSONParser";
	private JSONObject mjsonObj = null;

	private void logMsg(char type, String msg) {
		if (DEBUG && Const.DEBUG) {
			if (type == 'i')
				Log.i(TAG_NAME, msg);
			else if (type == 'w')
				Log.w(TAG_NAME, msg);
		}
	}

}
