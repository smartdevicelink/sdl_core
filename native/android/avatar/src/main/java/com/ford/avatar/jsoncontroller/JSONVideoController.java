package com.ford.avatar.jsoncontroller;

import java.io.File;
import android.content.SharedPreferences;
import android.util.Log;

import com.ford.avatar.activity.AvatarActivity;
import com.ford.avatar.downloader.DownloaderClient;
import com.ford.avatar.exception.SDCardException;
import com.ford.avatar.jsonparser.EVideoMethods;
import com.ford.avatar.util.Const;
import com.ford.avatar.util.ExtStorageUtils;
import com.ford.avatar.util.MessageConst;
import com.ford.avatar.util.NetUtils;
import com.ford.avatar.util.RPCConst;
import com.ford.avatar.util.Utils;
import com.ford.avatar.util.WebViewUtils;

/**
 * <b>File:</b> JSONVideoController.java <br>
 * <b>Description:</b> Controller that is responsible for video data
 * transferring through JSON RPC 2.0 protocol <br>
 * 
 * @author kbotnar
 * 
 */
public class JSONVideoController extends JSONController {

	/**
	 * Constructor
	 * 
	 * @param activity
	 *            : the application main activity
	 */
	public JSONVideoController(AvatarActivity activity) {
		super(RPCConst.CN_VIDEO);
		mActivity = activity;
	}

	/**
	 * Send notification to web UI with video progress bar position
	 * 
	 * @param position
	 *            : video progress bar position
	 */
	public void sendPositionNotification(int position) {
		String method = RPCConst.CN_VIDEO + "."
				+ EVideoMethods.positionChanged.toString();
		mJSONParser.putEmptyJSONObject();
		mJSONParser.putIntValue(RPCConst.TAG_VIDEO_CURRENT_POSITION, position);
		this.sendNotification(method, mJSONParser.getJSONObject());
	}

	/**
	 * Set started video duration
	 * 
	 * @param value
	 *            : video duration
	 */
	public void setVideoDuration(int value) {
		mVideoDuration = value;
	}

	// ===================================================================
	// private section
	// ===================================================================
	/**
	 * To show debug messages from current class
	 */
	private final boolean DEBUG = true;
	private final String TAG_NAME = JSONVideoController.class.getSimpleName();
	private AvatarActivity mActivity;
	/**
	 * Video file name in expansion file
	 */
	private String mVideoName;
	private int mVideoDuration = 0;
	private boolean mVideoIsStarted = false; // to prevent double start of video

	@Override
	protected void processRequest(String request) {
		logMsg("Process request");
		String result = processNotification(request);
		if (result != null) {
			mJSONParser.putJSONObject(request);
			sendResponse(mJSONParser.getId(), result);
		}
	}

	@Override
	protected String processNotification(String notification) {
		logMsg("Process notification : " + notification);
		mJSONParser.putJSONObject(notification);
		String method = mJSONParser.getMethod();
		method = method.substring(method.indexOf('.') + 1, method.length());
		String out = null;
		switch (EVideoMethods.valueOf(method)) {
		case start:
			mJSONParser.putJSONObject(mJSONParser.getParams());
			mVideoName = mJSONParser
					.getStringParam(RPCConst.TAG_VIDEO_FILE_NAME);
			double scale = mJSONParser.getDoubleParam(RPCConst.TAG_SCALE);
			mJSONParser.putJSONObject(mJSONParser
					.getJSONObjectParam(RPCConst.TAG_VIDEO_POSITION));
			int left = mJSONParser.getIntParam(RPCConst.TAG_VIDEO_POSITION_X);
			int top = mJSONParser.getIntParam(RPCConst.TAG_VIDEO_POSITION_Y);
			out = start(scale, left, top);
			break;
		case stop:
			out = stop();
			break;
		case pause:
			out = pause();
			break;
		case resume:
			out = resume();
			break;
		case setPositionPaused:
			mJSONParser.putJSONObject(mJSONParser.getParams());
			double pos = mJSONParser
					.getDoubleParam(RPCConst.TAG_VIDEO_POSITION);
			logMsg("POSITION = " + String.valueOf(pos));
			out = setPositionPaused(pos);
			break;
		case setPosition:
			mJSONParser.putJSONObject(mJSONParser.getParams());
			pos = mJSONParser.getDoubleParam(RPCConst.TAG_VIDEO_POSITION);
			logMsg("POSITION = " + String.valueOf(pos));
			out = setPosition(pos);
			break;
		case getPosition:
			out = getPosition();
			break;
		case stateChanged:
			out = stateChanged();
			break;
		case positionChanged:
			out = positionChanged();
			break;
		case setDragState:
			mJSONParser.putJSONObject(mJSONParser.getParams());
			boolean state = mJSONParser
					.getBooleanParam(RPCConst.TAG_VIDEO_DRAG_STATE);
			out = setDragState(state);
			break;
		default:
			mJSONParser.putEmptyJSONRPCObject();
			mJSONParser.putStringValue(RPCConst.TAG_ERROR,
					"VideoController does not support function : " + method);
			out = mJSONParser.getJSONObject();
			break;
		}
		return out;

	}

	@Override
	protected void processResponse(String response) {
		logMsg("Process response");
		if (!processRegistrationResponse(response)) {
			mJSONParser.putJSONObject(response);
			mRequestResponse = mJSONParser.getResult();
		}
	}

	/**
	 * Subscription on properties
	 */
	protected void subscribeToProperties() {
		subscribeTo("VideoPlayerClient.setDragState");
	}

	/**
	 * Start a video with parameters from web UI
	 * 
	 * @param scale
	 *            : video scale
	 * @param leftOffset
	 *            : left video offset
	 * @param topOffset
	 *            : top video offset
	 * @return result JSON object with video duration or error JSON object
	 */
	private String start(double scale, int leftOffset, int topOffset) {
		// check if expansion files are available
		SharedPreferences prefs = mActivity.getSharedPreferences(
				Const.SHPREF_DOWNLOADER_PREFS, 0);
		boolean expFielsAreValid = prefs.getBoolean(
				Const.SHPREF_MAIN_EXPFILE_VALID, false)
				&& prefs.getBoolean(Const.SHPREF_PATCH_EXPFILE_VALID, false);

		// additionally check if it is a welcome video
		if (expFielsAreValid
				|| mVideoName.toLowerCase().compareTo(
						Const.WELCOME_VIDEO_FILE_NAME) == 0) {
			if (!mVideoIsStarted) {
				mVideoIsStarted = true;
				logMsg("Play video " + mVideoName);
				Object obj[] = { mVideoName, scale, leftOffset, topOffset };
				Utils.sendMessageToHandler(Const.VIDEO_START, obj,
						mActivity.getMainHandler());
			}
			// wait for initializing video file and getting its duration
			while (mVideoDuration == 0) {
				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}

			if (mVideoDuration >= 0) {
				mJSONParser.putEmptyJSONObject();
				mJSONParser.putIntValue(RPCConst._TAG_VIDEO_TOTAL_DURATION,
						mVideoDuration);
				return mJSONParser.getJSONObject();
			}
		}
		return findVideoPlayError();
	}

	/**
	 * Finds out what is wrong with video playing
	 * 
	 * @return error JSON object
	 */
	private String findVideoPlayError() {
		mJSONParser.putEmptyJSONObject();
		//put error value as an indicator for parser
		mJSONParser.putStringValue(RPCConst.TAG_ERROR, "");
		// get redownload count
		SharedPreferences prefs = mActivity.getSharedPreferences(
				Const.SHPREF_DOWNLOADER_PREFS, 0);
		int redownload_count = prefs.getInt(Const.SHPREF_REDOWNLOAD_COUNTER, 0);

		// no network connection
		if (!NetUtils.isOnline(mActivity)) {
			mJSONParser.putStringValue(RPCConst.TAG_ERROR_MESSAGE,
					MessageConst.NET_NET_NOT_AVAILABLE);
			mJSONParser.putIntValue(RPCConst.TAG_ERROR_CODE,
					Const.VC_NO_NETWORK_ERR_CODE);
		} // video files are still downloading
		else if (DownloaderClient.IS_PROCESSING_VIDEO) {
			mJSONParser.putStringValue(RPCConst.TAG_ERROR_MESSAGE,
					MessageConst.VC_VIDEO_IS_BEING_PROCESSED_ERR);
			mJSONParser.putIntValue(RPCConst.TAG_ERROR_CODE,
					Const.VC_VIDEO_IS_BEING_DOWNLOADED_ERR_CODE);
			mJSONParser.putIntValue(RPCConst.TAG_ERROR_DATA,
					DownloaderClient.DOWNLOADED_PERSENS);
			DownloaderClient.FINISH_NOTIFY = true;
		}// no space on SD card
		else if (DownloaderClient.SDCARD_IS_FULL) {
			mJSONParser.putStringValue(RPCConst.TAG_ERROR_MESSAGE,
					MessageConst.VC_NO_SPACE_ERR);
			mJSONParser.putIntValue(RPCConst.TAG_ERROR_CODE,
					Const.VC_NO_SPACE_ERR_CODE);
		}// video downloading count is exceeded
		else if (redownload_count < Const.DOWNLOADER_LAUNCH_MAX_NUMBER) {
			mJSONParser.putStringValue(RPCConst.TAG_ERROR_MESSAGE,
					MessageConst.VC_VIDEO_IS_CORRUPTED_ERR);
			mJSONParser.putIntValue(RPCConst.TAG_ERROR_CODE,
					Const.VC_VIDEO_IS_CORRUPTED_ERR_CODE);
		} // video is unavailable due to file corruption
		else {
			mJSONParser.putStringValue(RPCConst.TAG_ERROR_MESSAGE,
					MessageConst.VC_VIDEO_IS_UNAVAILABLE_ERR);
			mJSONParser.putIntValue(RPCConst.TAG_ERROR_CODE,
					Const.VC_VIDEO_IS_UNAVAILABLE_ERR_CODE);
		}
		return mJSONParser.getJSONObject();
	}

	/**
	 * stop video playing
	 * 
	 * @return null, because it is a notification
	 */
	private String stop() {
		logMsg("VideoStop");
		mVideoIsStarted = false;
		Utils.sendMessageToHandler(Const.VIDEO_STOP, null,
				mActivity.getMainHandler());
		mVideoDuration = 0;
		return null;
	}

	/**
	 * pause video playing
	 * 
	 * @return null, because it is a notification
	 */
	private String pause() {
		logMsg("VideoPause");
		Utils.sendMessageToHandler(Const.VIDEO_PAUSE, null,
				mActivity.getMainHandler());
		return null;
	}

	/**
	 * resume video playing
	 * 
	 * @return null, because it is a notification
	 */
	private String resume() {
		logMsg("VideoPlay");
		Utils.sendMessageToHandler(Const.VIDEO_PLAY, null,
				mActivity.getMainHandler());
		return null;
	}

	/**
	 * Set a new position of video while it is paused
	 * 
	 * @param position
	 *            : new position
	 * @return null, because it is a notification
	 */
	private String setPositionPaused(double position) {
		logMsg("VideoSetPositionPaused:" + position + " convert to=" + position);
		Utils.sendMessageToHandler(Const.VIDEO_SET_POSITION_PAUSED, position,
				mActivity.getMainHandler());
		return null;
	}

	/**
	 * Set a new position of video while it is playing
	 * 
	 * @param position
	 *            : new position
	 * @return null, because it is a notification
	 */
	private String setPosition(double position) {
		logMsg("VideoSetPosition:" + position + " covert to=" + position);
		Utils.sendMessageToHandler(Const.VIDEO_SET_POSITION, position,
				mActivity.getMainHandler());
		return null;
	}

	/**
	 * Get current position in seconds
	 * 
	 * @return current position in seconds
	 */
	private String getPosition() {
		double out = mActivity.videoGetPosition();
		logMsg("VideoGetPosition Position=" + out);
		return String.valueOf(out);
	}

	/**
	 * Stub for future
	 * 
	 * @return null
	 */
	private String stateChanged() {
		return null;
	}

	/**
	 * Stub for future
	 * 
	 * @return null
	 */
	private String positionChanged() {
		return null;
	}

	/**
	 * Disable and enable dragging in the application. It should be available
	 * only during video playing
	 * 
	 * @param activate
	 *            : activates dragging
	 * @return null, because it is a notification
	 */
	private String setDragState(boolean activate) {
		if (activate) {
			// allow dragging only if dragging video
			WebViewUtils.setOnTouchListener(mActivity,
					WebViewUtils.NONBLOCKING_ACTION_MOVE_MODE);
		} else {
			// disable dragging when stop scrolling video
			WebViewUtils.setOnTouchListener(mActivity,
					WebViewUtils.BLOCKING_ACTION_MOVE_MODE);
		}
		return null;
	}

	/**
	 * Check if file exists on SD card.
	 * 
	 * @param fileName
	 *            : file name
	 * @return true if file exists and false otherwise.
	 */
	public boolean isFileExist(String fileName) {
		if (fileName == null || "".equals(fileName.trim())) {
			return false;
		}
		try {

			File file = ExtStorageUtils.getFileOnSDCard(mActivity,
					fileName.toLowerCase());
			return file.exists();
		} catch (SDCardException e) {
			e.printStackTrace();
			return false;
		}
	}

	private void logMsg(String msg) {
		if (DEBUG && Const.DEBUG) {
			Log.i(TAG_NAME, msg);
		}
	}

}
