package com.ford.syncV4.android.adapters;

import android.util.Log;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.android.activity.PlaceholderFragment;
import com.ford.syncV4.util.logger.Logger;

import java.util.ArrayList;

public class LogAdapter extends MessageAdapter {
    
    private String mLogTag;
    private boolean mFullUIDebug;
    /**
     * Application Id associated with the concrete Fragment
     */
    private String mAppId = PlaceholderFragment.EMPTY_APP_ID;

    public LogAdapter(String logTag, boolean fullUIDebug, int textViewResourceId,
                      ArrayList<Object> items) {
        super(MainApp.getInstance(), textViewResourceId, items);
        mLogTag = logTag;
        mFullUIDebug = fullUIDebug;
    }

    public void logMessage(final Object m) {
        if (m == null) {
            return;
        }
       Logger.i(mLogTag, m.toString());
        if (mFullUIDebug) {
            addMessageToUI(m);
        }
    }

    public void logMessage(Object m, Boolean addToUI) {
        if (m == null) {
            return;
        }
       Logger.i(mLogTag, m.toString());
        if (addToUI) {
            addMessageToUI(m);
        }
    }

    public void logMessage(final Object m, Integer type) {
        if (m instanceof String) {
            switch (type) {
                case Log.DEBUG:
                   Logger.d(mLogTag, m.toString());
                    break;
                case Log.ERROR:
                   Logger.e(mLogTag, m.toString());
                    break;
                case Log.VERBOSE:
                   Logger.d(mLogTag, m.toString());
                    break;
                case Log.WARN:
                   Logger.w(mLogTag, m.toString());
                    break;
                default:
                   Logger.i(mLogTag, m.toString());
                    break;
            }
        }
        if (mFullUIDebug) {
            addMessageToUI(m);
        }
    }

    public void logMessage(final Object m, Integer type, Boolean addToUI) {
        if (m instanceof String) {
            switch (type) {
                case Log.DEBUG:
                   Logger.d(mLogTag, m.toString());
                    break;
                case Log.ERROR:
                   Logger.e(mLogTag, m.toString());
                    break;
                case Log.VERBOSE:
                   Logger.d(mLogTag, m.toString());
                    break;
                case Log.WARN:
                   Logger.w(mLogTag, m.toString());
                    break;
                default:
                   Logger.i(mLogTag, m.toString());
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
                   Logger.d(mLogTag, m.toString());
                    break;
                case Log.ERROR:
                   Logger.e(mLogTag + " " + m.toString(), tr);
                    break;
                case Log.VERBOSE:
                   Logger.d(mLogTag, m.toString());
                    break;
                case Log.WARN:
                   Logger.w(mLogTag, m.toString());
                    break;
                default:
                   Logger.i(mLogTag, m.toString());
                    break;
            }
        }
        if (mFullUIDebug) {
            addMessageToUI(m);
        }
    }

    public void logMessage(final Object m, Integer type, Throwable tr, Boolean addToUI) {
        if (m instanceof String) {
            switch (type) {
                case Log.DEBUG:
                   Logger.d(mLogTag, m.toString());
                    break;
                case Log.ERROR:
                   Logger.e(mLogTag +" " + m.toString(), tr);
                    break;
                case Log.VERBOSE:
                   Logger.d(mLogTag, m.toString());
                    break;
                case Log.WARN:
                   Logger.w(mLogTag, m.toString());
                    break;
                default:
                   Logger.i(mLogTag, m.toString());
                    break;
            }
        }
        if (addToUI) {
            addMessageToUI(m);
        }
    }

    public String getAppId() {
        return mAppId;
    }

    public void setAppId(String value) {
        if (value == null) {
            return;
        }
        mAppId = value;
    }

    private void addMessageToUI(final Object m) {
        MainApp.getInstance().runInUIThread(new Runnable() {
            public void run() {
                addMessage(m);
            }
        });
    }
}