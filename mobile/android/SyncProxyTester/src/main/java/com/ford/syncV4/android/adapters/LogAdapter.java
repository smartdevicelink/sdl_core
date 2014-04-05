package com.ford.syncV4.android.adapters;

import android.app.Activity;
import android.util.Log;

import com.ford.syncV4.util.logger.Logger;

import java.util.ArrayList;

public class LogAdapter extends MessageAdapter {
    
    private String logTag;
    boolean fullUIDebug;
    Activity activity;

    public LogAdapter(String logTag, boolean fullUIDebug, Activity activity,
                      int textViewResourceId, ArrayList<Object> items) {
        super(activity, textViewResourceId, items);
        this.activity = activity;
        this.logTag = logTag;
        this.fullUIDebug = fullUIDebug;
    }

    private void addMessageToUI(final Object m) {
        activity.runOnUiThread(new Runnable() {
            public void run() {
                addMessage(m);
            }
        });
    }

    public void logMessage(final Object m) {
        if (m == null) {
            return;
        }
       Logger.i(logTag, m.toString());
        if (fullUIDebug) {
            addMessageToUI(m);
        }
    }

    public void logMessage(Object m, Boolean addToUI) {
        if (m == null) {
            return;
        }
       Logger.i(logTag, m.toString());
        if (addToUI) {
            addMessageToUI(m);
        }
    }

    public void logMessage(final Object m, Integer type) {
        if (m instanceof String) {
            switch (type) {
                case Log.DEBUG:
                   Logger.d(logTag, m.toString());
                    break;
                case Log.ERROR:
                   Logger.e(logTag, m.toString());
                    break;
                case Log.VERBOSE:
                   Logger.d(logTag, m.toString());
                    break;
                case Log.WARN:
                   Logger.w(logTag, m.toString());
                    break;
                default:
                   Logger.i(logTag, m.toString());
                    break;
            }
        }
        if (fullUIDebug) {
            addMessageToUI(m);
        }
    }

    public void logMessage(final Object m, Integer type, Boolean addToUI) {
        if (m instanceof String) {
            switch (type) {
                case Log.DEBUG:
                   Logger.d(logTag, m.toString());
                    break;
                case Log.ERROR:
                   Logger.e(logTag, m.toString());
                    break;
                case Log.VERBOSE:
                   Logger.d(logTag, m.toString());
                    break;
                case Log.WARN:
                   Logger.w(logTag, m.toString());
                    break;
                default:
                   Logger.i(logTag, m.toString());
                    break;
            }
        }
        if (addToUI) {
            addMessageToUI(m);
        }
    }

    public void logMessage(final Object m, Integer type, Throwable tr) {
        if (m instanceof String) {
            switch (type) {
                case Log.DEBUG:
                   Logger.d(logTag, m.toString());
                    break;
                case Log.ERROR:
                   Logger.e(logTag + " " + m.toString(), tr);
                    break;
                case Log.VERBOSE:
                   Logger.d(logTag, m.toString());
                    break;
                case Log.WARN:
                   Logger.w(logTag, m.toString());
                    break;
                default:
                   Logger.i(logTag, m.toString());
                    break;
            }
        }
        if (fullUIDebug) {
            addMessageToUI(m);
        }
    }

    public void logMessage(final Object m, Integer type, Throwable tr, Boolean addToUI) {
        if (m instanceof String) {
            switch (type) {
                case Log.DEBUG:
                   Logger.d(logTag, m.toString());
                    break;
                case Log.ERROR:
                   Logger.e(logTag +" " + m.toString(), tr);
                    break;
                case Log.VERBOSE:
                   Logger.d(logTag, m.toString());
                    break;
                case Log.WARN:
                   Logger.w(logTag, m.toString());
                    break;
                default:
                   Logger.i(logTag, m.toString());
                    break;
            }
        }
        if (addToUI) {
            addMessageToUI(m);
        }
    }
}