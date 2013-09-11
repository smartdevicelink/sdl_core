package com.ford.syncV4.android.activity.mobilenav;

/**
 * Created by Andrew Batutin on 9/11/13.
 */
public interface DataReaderListener {

    public void onStartReading();
    public void onDataReceived(final byte [] data);
    public void onCancelReading();
    public void onEndReading();
}
