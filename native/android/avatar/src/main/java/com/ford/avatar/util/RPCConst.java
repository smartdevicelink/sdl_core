package com.ford.avatar.util;

public class RPCConst {
	// =========================================================================================
	// Constants for JSON modules
	// =========================================================================================

	// general tags
	public static final String TAG_JSONRPC_VERSION = "jsonrpc";
	public static final String TAG_JSONRPC_VERSION_VALUE = "2.0";
	public static final String TAG_METHOD = "method";
	public static final String TAG_PARAMETERS = "params";
	public static final String TAG_ID = "id";
	// response tags
	public static final String TAG_RESULT = "result";
	public static final String TAG_ERROR = "error";
	// error tags
	public static final String TAG_ERROR_CODE = "code";
	public static final String TAG_ERROR_MESSAGE = "message";
	public static final String TAG_ERROR_DATA = "data";
	// additional info tags
	public static final String TAG_TYPE = "type";
	public static final String TAG_TYPE_OF_CONTACTS = "typeOfContacts";
	public static final String TAG_FIRST_LETTER = "firstLetter";
	public static final String TAG_CONTACT_ID = "contactId";
	public static final String TAG_FIRST_NAME = "firstName";
	public static final String TAG_LAST_NAME = "lastName";
	public static final String TAG_PHONE_NUMBER = "phoneNumber";
	public static final String TAG_TIME = "time";
	public static final String TAG_RATE_CHOICE = "choice";
	
	public static final String TAG_VIDEO_CURRENT_POSITION = "currentPosition";
	public static final String TAG_VIDEO_FILE_NAME = "videoName";
	public static final String TAG_VIDEO_POSITION = "position";
	public static final String TAG_VIDEO_POSITION_X = "x";
	public static final String TAG_VIDEO_POSITION_Y = "y";
	public static final String TAG_VIDEO_DRAG_STATE = "dragState";
	public static final String _TAG_VIDEO_TOTAL_DURATION = "totalDuration";
	
	//components names
	public static final String CN_AVATAR = "AVA";
	public static final String CN_MESSAGE_BROKER = "MB";
	public static final String CN_PHONE = "Phone";
	public static final String CN_BACKEND = "Backend";
	public static final String CN_VIDEO = "VideoPlayer";
	public static final String CN_BACKEND_CLIENT = "BackendClient";
	public static final String CN_RATE = "Rate";
	public static final String CN_RATE_CLIENT = "RateClient";
	
	//notification tags
	public static final String TAG_CALL_STATUS = "callStatus";

	// server constants
	public static final int TCP_SERVER_PORT = 21111;
	public static final String LOCALHOST = "localhost";

	public static final int ID_RANGE = 999;
	public static final String TAG_SCALE = "scale";

	
	

}
