/**
 * 
 */
package com.ford.avatar.test.jsonparser;

import org.json.JSONException;
import org.json.JSONObject;

import com.ford.avatar.jsonparser.JSONParser;
import com.ford.avatar.util.Const;
import com.ford.avatar.util.RPCConst;

import android.test.AndroidTestCase;

/**
 * @author KBotnar
 * 
 */
public class JSONParserTest extends AndroidTestCase {

	private JSONParser mParser;

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.test.AndroidTestCase#setUp()
	 */
	protected void setUp() throws Exception {
		super.setUp();
		mParser = new JSONParser();
		mParser.putEmptyJSONObject();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.test.AndroidTestCase#tearDown()
	 */
	protected void tearDown() throws Exception {
		super.tearDown();
	}

	/**
	 * Test method for
	 * {@link com.ford.dtest.jsonparser.JSONParser#JSONParser(java.lang.String)}
	 * .
	 */
	public void testJSONParserString() {
		JSONParser parser = new JSONParser("test");
		assertNotNull(parser);
	}

	/**
	 * Test method for
	 * {@link com.ford.dtest.jsonparser.JSONParser#JSONParser(org.json.JSONObject)}
	 * .
	 */
	public void testJSONParserJSONObject() {
		JSONParser parser = new JSONParser(new JSONObject());
		assertNotNull(parser);
	}

	/**
	 * Test method for
	 * {@link com.ford.dtest.jsonparser.JSONParser#JSONParser()}.
	 */
	public void testJSONParser() {
		JSONParser parser = new JSONParser();
		assertNotNull(parser);
	}

	/**
	 * Test method for
	 * {@link com.ford.dtest.jsonparser.JSONParser#createJSONServerRequest(java.lang.String, org.json.JSONObject, java.lang.Integer)}
	 * .
	 */
	public void testCreateJSONServerRequestStringJSONObjectInteger() {
		Integer id = 12;
		String method = "testMethod";
		JSONObject params = new JSONObject();
		String name = "testname";
		String value = "testvalue";
		try {
			params.put(name, value);
			String actual = mParser.createJSONServerRequest(method, params, id);

			JSONObject expected = new JSONObject();
			expected.put(RPCConst.TAG_ID, id);
			expected.put(RPCConst.TAG_METHOD, method);
			expected.put(RPCConst.TAG_PARAMETERS, params);
			String expStr = expected.toString()
					+ System.getProperty("line.separator");
			assertEquals(expStr, actual);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Test method for
	 * {@link com.ford.dtest.jsonparser.JSONParser#createJSONServerRequest(java.lang.String, java.lang.String, java.lang.Integer)}
	 * .
	 */
	public void testCreateJSONServerRequestStringStringInteger() {
		Integer id = 12;
		String method = "testMethod";
		JSONObject params = new JSONObject();
		String name = "testname";
		String value = "testvalue";
		try {
			params.put(name, value);
			String actual = mParser.createJSONServerRequest(method,
					params.toString(), id);

			JSONObject expected = new JSONObject();
			expected.put(RPCConst.TAG_ID, id);
			expected.put(RPCConst.TAG_METHOD, method);
			expected.put(RPCConst.TAG_PARAMETERS, params);
			String expStr = expected.toString()
					+ System.getProperty("line.separator");
			assertEquals(expStr, actual);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Test method for
	 * {@link com.ford.dtest.jsonparser.JSONParser#createJSONResponse(int, java.lang.String)}
	 * .
	 */
	public void testCreateJSONResponse() {
		int id = 1;
		JSONObject result = new JSONObject();
		String name = "testname";
		String value = "testvalue";
		try {
			result.put(name, value);
			String actual = mParser.createJSONResponse(id, result.toString());

			JSONObject expected = new JSONObject();
			expected.put(RPCConst.TAG_ID, id);
			expected.put(RPCConst.TAG_RESULT, result);
			String expStr = expected.toString()
					+ System.getProperty("line.separator");
			assertEquals(expStr, actual);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Test method for
	 * {@link com.ford.dtest.jsonparser.JSONParser#createJSONNotification(java.lang.String, java.lang.String)}
	 * .
	 */
	public void testCreateJSONNotification() {
		String method = "testMethod";
		JSONObject params = new JSONObject();
		String name = "testname";
		String value = "testvalue";
		try {
			params.put(name, value);
			String actual = mParser.createJSONNotification(method,
					params.toString());

			JSONObject expected = new JSONObject();
			expected.put(RPCConst.TAG_METHOD, method);
			expected.put(RPCConst.TAG_PARAMETERS, params);
			String expStr = expected.toString()
					+ System.getProperty("line.separator");
			assertEquals(expStr, actual);
		} catch (JSONException e) {
			e.printStackTrace();
		}

	}

	/**
	 * Test method for
	 * {@link com.ford.dtest.jsonparser.JSONParser#putJSONObject(java.lang.String)}
	 * .
	 */
	public void testPutJSONObjectString() {
		try {
			JSONObject expected = new JSONObject("test:2");
			mParser.putJSONObject(expected.toString());
			JSONObject actual = new JSONObject(mParser.getJSONObject());
			assertEquals(expected, actual);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Test method for
	 * {@link com.ford.dtest.jsonparser.JSONParser#putEmptyJSONRPCObject()}.
	 */
	public void testPutEmptyJSONRPCObject() {
		mParser.putEmptyJSONRPCObject();
		JSONObject expected = new JSONObject();
		try {
			expected.put(RPCConst.TAG_JSONRPC_VERSION,
					RPCConst.TAG_JSONRPC_VERSION_VALUE);
		} catch (JSONException e1) {
			e1.printStackTrace();
		}
		String actual = mParser.getJSONObject();
		assertEquals(expected.toString(), actual);
	}

	/**
	 * Test method for
	 * {@link com.ford.dtest.jsonparser.JSONParser#putEmptyJSONObject()}.
	 */
	public void testPutEmptyJSONObject() {
		mParser.putEmptyJSONObject();
		String expected = new JSONObject().toString();
		String actual = mParser.getJSONObject();
		assertEquals(expected, actual);
	}

	/**
	 * Test method for
	 * {@link com.ford.dtest.jsonparser.JSONParser#putJSONObject(org.json.JSONObject)}
	 * .
	 */
	public void testPutJSONObjectJSONObject() {
		JSONObject expected;
		try {
			expected = new JSONObject("test");
			mParser.putJSONObject(expected);
			String actual = mParser.getJSONObject();
			assertEquals(expected.toString(), actual);
		} catch (JSONException e) {
			e.printStackTrace();
		}

	}

	/**
	 * Test method for
	 * {@link com.ford.dtest.jsonparser.JSONParser#putStringValue(java.lang.String, java.lang.String)}
	 * .
	 */
	public void testPutStringValue() {
		String expected = "test";
		mParser.putStringValue("name", expected);
		String actual = mParser.getStringParam("name");
		assertEquals(expected, actual);
	}

	/**
	 * Test method for
	 * {@link com.ford.dtest.jsonparser.JSONParser#putIntValue(java.lang.String, int)}
	 * .
	 */
	public void testPutIntValue() {
		int expected = 11;
		mParser.putIntValue("name", expected);
		int actual = mParser.getIntParam("name");
		assertEquals(expected, actual);
	}

	/**
	 * Test method for
	 * {@link com.ford.dtest.jsonparser.JSONParser#putBooleanValue(java.lang.String, boolean)}
	 * .
	 */
	public void testPutBooleanValue() {
		boolean expected = true;
		mParser.putBooleanValue("name", expected);
		boolean actual = mParser.getBooleanParam("name");
		assertEquals(expected, actual);
	}

	/**
	 * Test method for
	 * {@link com.ford.dtest.jsonparser.JSONParser#putDoubleValue(java.lang.String, double)}
	 * .
	 */
	public void testPutDoubleValue() {
		double expected = 0.1;
		mParser.putDoubleValue("name", expected);
		double actual = mParser.getDoubleParam("name");
		assertEquals(expected, actual);
	}

}
