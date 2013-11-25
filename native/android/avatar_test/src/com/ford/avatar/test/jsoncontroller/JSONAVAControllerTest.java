/**
 * 
 */
package com.ford.avarsdl.test.jsoncontroller;

import org.json.JSONException;
import org.json.JSONObject;

import com.ford.avarsdl.jsoncontroller.JSONAVAController;
import com.ford.avarsdl.test.stubs.TCPClientStub;
import com.ford.avarsdl.util.RPCConst;

import junit.framework.TestCase;

/**
 * @author kbotnar
 *
 */
public class JSONAVAControllerTest extends TestCase {

	
	private TCPClientStub mTcpStub;
	private final int ID = 100;
	private JSONAVAController mController;

	/* (non-Javadoc)
	 * @see junit.framework.TestCase#setUp()
	 */
	protected void setUp() throws Exception {
		super.setUp();
		mTcpStub = new TCPClientStub(RPCConst.LOCALHOST,
				RPCConst.TCP_SERVER_PORT);
		mController = new JSONAVAController(RPCConst.CN_AVATAR, mTcpStub);

	}

	/* (non-Javadoc)
	 * @see junit.framework.TestCase#tearDown()
	 */
	protected void tearDown() throws Exception {
		super.tearDown();
	}

	/**
	 * Test method for {@link com.ford.avarsdl.jsoncontroller.JSONAVAController#processRequest(java.lang.String)}.
	 */
	public final void testProcessRequest() {
		fail("Not yet implemented"); // TODO
	}

	/**
	 * Test method for {@link com.ford.avarsdl.jsoncontroller.JSONAVAController#processResponse(java.lang.String)}.
	 */
	public final void testProcessResponse() {
		fail("Not yet implemented"); // TODO
	}

	/**
	 * Test method for {@link com.ford.avarsdl.jsoncontroller.JSONAVAController#JSONAVAController(com.ford.avarsdl.activity.AvatarActivity, java.lang.String)}.
	 */
	public final void testJSONAVAController() {
		JSONAVAController controller = new JSONAVAController(RPCConst.CN_AVATAR, mTcpStub);
		assertNotNull(controller);
	}

	/**
	 * Test method for {@link com.ford.avarsdl.jsoncontroller.JSONAVAController#sendJSMessage(java.lang.String, java.lang.String)}.
	 */
	public final void testSendJSMessage() {
		JSONObject obj = new JSONObject();
		try {
			obj.put(RPCConst.TAG_ID, ID);
			mController.sendJSMessage(RPCConst.CN_AVATAR, obj.toString());
			int actual = new JSONObject(mTcpStub.getMessage()).getInt(RPCConst.TAG_ID);
			assertEquals(ID, actual);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		
	}

}
