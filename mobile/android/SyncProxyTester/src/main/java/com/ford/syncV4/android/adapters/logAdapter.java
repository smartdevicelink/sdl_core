package com.ford.syncV4.android.adapters;

import java.util.ArrayList;

import android.app.Activity;
import android.util.Log;

public class logAdapter extends MessageAdapter {
	private String logTag;
	boolean fullUIDebug;
	Activity activity;

	public logAdapter(String logTag, boolean fullUIDebug, Activity activity, int textViewResourceId, ArrayList<Object> items) {
		super(activity, textViewResourceId, items);
		this.activity = activity;
		this.logTag = logTag;
		this.fullUIDebug = fullUIDebug;
	}
	
    private void addMessageToUI(final Object m) {    	
    	activity.runOnUiThread(new Runnable() {
			public void run() { addMessage(m); }
		});
    }
    
    public void logMessage (final Object m) {
		Log.i(logTag, m.toString());
    	if(fullUIDebug) addMessageToUI(m);
	}
    public void logMessage (final Object m, Boolean addToUI) {
		Log.i(logTag, m.toString());		
    	addMessageToUI(m);
    }
    public void logMessage (final Object m, Integer type) {
    	if (m instanceof String) {
    		switch(type) {
	    		case Log.DEBUG:
	    			Log.d(logTag, m.toString());
	    			break;
	    		case Log.ERROR:
	    			Log.e(logTag, m.toString());
	    			break;
	    		case Log.VERBOSE:
	    			Log.v(logTag, m.toString());
	    			break;
	    		case Log.WARN:
	    			Log.w(logTag, m.toString());
	    			break;
	    		default:
	    			Log.i(logTag, m.toString());
	    			break;
    		}
    	}
    	if (fullUIDebug) addMessageToUI(m);
	}
    public void logMessage (final Object m, Integer type, Boolean addToUI) {
    	if (m instanceof String) {
    		switch(type) {
	    		case Log.DEBUG:
	    			Log.d(logTag, m.toString());
	    			break;
	    		case Log.ERROR:
	    			Log.e(logTag, m.toString());
	    			break;
	    		case Log.VERBOSE:
	    			Log.v(logTag, m.toString());
	    			break;
	    		case Log.WARN:
	    			Log.w(logTag, m.toString());
	    			break;
	    		default:
	    			Log.i(logTag, m.toString());
	    			break;
    		}
    	}
    	if (addToUI) addMessageToUI(m);
	}
    public void logMessage (final Object m, Integer type, Throwable tr) {
    	if (m instanceof String) {
    		switch(type) {
	    		case Log.DEBUG:
	    			Log.d(logTag, m.toString());
	    			break;
	    		case Log.ERROR:
	    			Log.e(logTag, m.toString(), tr);
	    			break;
	    		case Log.VERBOSE:
	    			Log.v(logTag, m.toString());
	    			break;
	    		case Log.WARN:
	    			Log.w(logTag, m.toString());
	    			break;
	    		default:
	    			Log.i(logTag, m.toString());
	    			break;
    		}
    	}
    	if (fullUIDebug) addMessageToUI(m);
	}
    public void logMessage (final Object m, Integer type, Throwable tr, Boolean addToUI) {
    	if (m instanceof String) {
    		switch(type) {
	    		case Log.DEBUG:
	    			Log.d(logTag, m.toString());
	    			break;
	    		case Log.ERROR:
	    			Log.e(logTag, m.toString(), tr);
	    			break;
	    		case Log.VERBOSE:
	    			Log.v(logTag, m.toString());
	    			break;
	    		case Log.WARN:
	    			Log.w(logTag, m.toString());
	    			break;
	    		default:
	    			Log.i(logTag, m.toString());
	    			break;
    		}
    	}
    	if (addToUI) addMessageToUI(m);
	}
}