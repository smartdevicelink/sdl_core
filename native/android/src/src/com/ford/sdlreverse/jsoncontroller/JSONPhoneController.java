package com.ford.sdlreverse.jsoncontroller;



import java.text.SimpleDateFormat;
import java.util.Calendar;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;


import com.ford.sdlreverse.activity.AvatarActivity;
import com.ford.sdlreverse.jsonparser.EBEMethods;
import com.ford.sdlreverse.jsonparser.EPhCMethods;
import com.ford.sdlreverse.util.Const;
import com.ford.sdlreverse.util.RPCConst;

import android.database.ContentObserver;
import android.database.Cursor;
import android.os.Message;
import android.provider.CallLog;
import android.provider.ContactsContract;
import android.util.Log;

public class JSONPhoneController extends JSONController {

	public JSONPhoneController(AvatarActivity activity) {
		super(RPCConst.CN_PHONE);
		mActivity = activity;
		mContentObserver = new ContentObserver(null) {
			@Override
			public void onChange(boolean selfChange) {
				super.onChange(selfChange);
				// update contacts
				updateContacts();
			}

		};

	}

	public void sendEndCallNotification(boolean value) {
		String method = RPCConst.CN_PHONE + "."
				+ EPhCMethods.onCallIsEnded.toString();
		mJSONParser.putEmptyJSONObject();
		mJSONParser.putBooleanValue("callIsEnded", value);
		sendNotification(method, mJSONParser.getJSONObjectAsString());

	}

	// =======================================================
	// private and protected section
	// =======================================================
	// private String mResponse;
	private final boolean DEBUG = false;
	private final String TAG_NAME = JSONPhoneController.class.getSimpleName();// on
																				// and
																				// off
																				// logs
	private AvatarActivity mActivity;
	private ContentObserver mContentObserver;
	private Cursor mContactsQuery = null;
	private Cursor mHistoryQuery = null;
	private String mFirstLetters = null;
	private String mRange = null;// two first letters

	protected void processRequest(String request) {
		String result = processNotification(request);
		mJSONParser.putJSONObject(request);
		sendResponse(mJSONParser.getId(), result);
	}

	protected String processNotification(String notification) {
		mJSONParser.putJSONObject(notification);
		String method = mJSONParser.getMethod();
		method = method.substring(method.indexOf('.') + 1, method.length());
		String out = null;
		// get params
		mJSONParser.putJSONObject(mJSONParser.getParams());
		switch (EPhCMethods.valueOf(method)) {
		case getContacts:
			String firstLetters = mJSONParser.getStringParam(RPCConst.TAG_RANGE);
			Integer offset = mJSONParser.getIntParam(RPCConst.TAG_OFFSET);
			Integer numberOfItems = mJSONParser
					.getIntParam(RPCConst.TAG_NUMBER_OF_ITEMS);
			out = getContacts(firstLetters, offset, numberOfItems).toString();
			break;
		case makeCall:
			String phNumber = mJSONParser
					.getStringParam(RPCConst.TAG_PHONE_NUMBER);
			out = makeCall(phNumber);

			break;
		case getHistory:
			int quantity = mJSONParser.getIntParam(RPCConst.TAG_CONTACTS_QUANTITY);
			try {
				out = getHistory(quantity).toString();
			} catch (NumberFormatException e) {
				e.printStackTrace();
				out = "!!! Error: Wrong type of parameter for PhoneProxy.GetHistory(int q)";
			}
			break;
		default:
			mJSONParser.putEmptyJSONObject();
			mJSONParser.putStringValue(RPCConst.TAG_ERROR,
					"Beckend does not support function : " + method);
			out = mJSONParser.getJSONObjectAsString();
			break;
		}
		return out;
	}

	protected void processResponse(String response) {
		if (!processRegistrationResponse(response)) {
			mJSONParser.putJSONObject(response);
			mRequestResponse = mJSONParser.getResult();
		}
	}

	private String makeCall(String phoneNumber) {
		Message msg = mActivity.getMainHandler().obtainMessage(Const.MAKE_CALL,
				phoneNumber);
		mActivity.getMainHandler().sendMessage(msg);

		mJSONParser.putEmptyJSONObject();
		mJSONParser.putStringValue("result", "Success");
		return mJSONParser.getJSONObjectAsString();
	}

	private void updateContacts() {
		String[] projection = new String[] { ContactsContract.Contacts._ID,
				ContactsContract.Contacts.DISPLAY_NAME,
				ContactsContract.Contacts.HAS_PHONE_NUMBER,
				ContactsContract.Contacts.PHOTO_ID };
		String strOrder = ContactsContract.Contacts.DISPLAY_NAME + " ASC";
		String contactName = ContactsContract.Contacts.DISPLAY_NAME;
		String selection = null;
		String[] queryParams = null;
		if (mFirstLetters.equalsIgnoreCase("other")) {
			selection = contactName + " NOT BETWEEN 'A' AND 'Z' AND " + 
					contactName + " NOT BETWEEN 'a' AND 'z' AND " +
					contactName + " NOT LIKE 'Z%' AND " + contactName + " NOT LIKE 'z%'";
		} else {
			String flUp = mFirstLetters.toUpperCase();
			String flLow = mFirstLetters.toLowerCase();
			queryParams = new String[] { flUp.charAt(0) + "%",
					flUp.charAt(1) + "%", flLow.charAt(0) + "%",
					flLow.charAt(1) + "%", flUp.charAt(1) + "%",
					flLow.charAt(1) + "%" };
			selection = contactName + " BETWEEN ? AND ? OR " + contactName
					+ " BETWEEN ? AND ? OR " + contactName + " LIKE ? OR "
					+ contactName + " LIKE ?";
		}
		mContactsQuery = mActivity.getContentResolver().query(
				ContactsContract.Contacts.CONTENT_URI, projection, selection,
				queryParams, strOrder);

		mContactsQuery.moveToFirst();
	}

	private String getContacts(String firstLetters, Integer offsset,
			Integer numberOfItems) {
		// update contacts if it is a new query
		if (mContactsQuery == null || firstLetters != mFirstLetters) {
			mFirstLetters = firstLetters;
			updateContacts();
		}

		// get offset and number of items
		if (offsset == null)
			offsset = 0;
		if (numberOfItems == null || numberOfItems == 0)
			numberOfItems = mContactsQuery.getCount();

		JSONArray result = new JSONArray();

		// go to the first row
		mContactsQuery.moveToFirst();
		// skip offset
		if (mContactsQuery.moveToPosition(offsset.intValue())) {
			// get data
			do {
				mJSONParser.putEmptyJSONObject();
				String contactName = mContactsQuery
						.getString(mContactsQuery
								.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME));
				mJSONParser.putStringValue(RPCConst.TAG_FIRST_NAME, contactName);
				// check if contact has a phone number
				if (mContactsQuery
						.getInt(mContactsQuery
								.getColumnIndex(ContactsContract.Contacts.HAS_PHONE_NUMBER)) == 1) {
					// get phone number
					String[] phProjection = new String[] {
							ContactsContract.CommonDataKinds.Phone._ID,
							ContactsContract.CommonDataKinds.Phone.NUMBER };
					String phID = mContactsQuery.getString(mContactsQuery
							.getColumnIndex(ContactsContract.Contacts._ID));
					mJSONParser.putIntValue(RPCConst.TAG_CONTACT_ID,
							Integer.parseInt(phID));
					String[] phSelectionArg = new String[] { phID };
					Cursor phQuery = mActivity.getContentResolver().query(
							ContactsContract.CommonDataKinds.Phone.CONTENT_URI,
							phProjection,
							ContactsContract.CommonDataKinds.Phone.CONTACT_ID
									+ " = ?", phSelectionArg, null);
					if (phQuery.moveToFirst()) {
						String phoneNumber = phQuery
								.getString(phQuery
										.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER));
						mJSONParser.putStringValue(RPCConst.TAG_PHONE_NUMBER,
								phoneNumber);
					}
					phQuery.close();
					result.put(mJSONParser.getJSONObject());
				}
				numberOfItems--;
			} while (mContactsQuery.moveToNext()
					&& numberOfItems.intValue() > 0);
		}// if
		mJSONParser.putEmptyJSONObject();
		mJSONParser.putJSONObjectsArray("contacts", result);
		return mJSONParser.getJSONObjectAsString();
	}

	private String getHistory(int quantity) {

		JSONArray result = new JSONArray();

		String[] projection = new String[] { CallLog.Calls._ID,
				CallLog.Calls.CACHED_NAME, CallLog.Calls.NUMBER,
				CallLog.Calls.DATE, CallLog.Calls.DURATION };

		Cursor query = mActivity.getContentResolver().query(
				CallLog.Calls.CONTENT_URI, projection, null, null, null);

		while (query.moveToNext()) {
			mJSONParser.putEmptyJSONObject();
			int id = query.getInt(query.getColumnIndex(CallLog.Calls._ID));
			mJSONParser.putIntValue(RPCConst.TAG_CONTACT_ID, id);
			String phNumber = query.getString(query
					.getColumnIndex(CallLog.Calls.NUMBER));
			mJSONParser.putStringValue(RPCConst.TAG_PHONE_NUMBER, phNumber);
			String name = query.getString(query
					.getColumnIndex(CallLog.Calls.CACHED_NAME));
			mJSONParser.putStringValue(RPCConst.TAG_FIRST_NAME, name);
			long date = query.getLong(query.getColumnIndex(CallLog.Calls.DATE));
			mJSONParser.putStringValue(RPCConst.TAG_TIME, getDate(date));
			result.put(mJSONParser.getJSONObjectAsString());
		}

		return result.toString();
	}

	/**
	 * Return date in specified format.
	 * 
	 * @param milliSeconds
	 *            Date in milliseconds
	 * @param dateFormat
	 *            Date format
	 * @return String representing date in specified format
	 */
	private String getDate(long milliSeconds) {
		// Create a DateFormatter object for displaying date in specified
		// format.
		SimpleDateFormat formatter = new SimpleDateFormat("dd/MM/yyyy hh:mm:ss");

		// Create a calendar object that will convert the date and time value in
		// milliseconds to date.
		Calendar calendar = Calendar.getInstance();
		calendar.setTimeInMillis(milliSeconds);
		return formatter.format(calendar.getTime());
	}

	private void logMsg(char type, String msg) {
		if (DEBUG && Const.DEBUG) {
			if (type == 'i')
				Log.i(TAG_NAME, msg);
			else if (type == 'w')
				Log.w(TAG_NAME, msg);
		}
	}

}
