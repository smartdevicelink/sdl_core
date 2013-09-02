package com.ford.syncV4.android.activity.mobilenav;

/**
 * Created by Andrew Batutin on 9/2/13.
 */
public interface VideoDataListener {
    public void  onStreamingStart();
    public  void videoFrameReady(byte [] videoFrame);
}
