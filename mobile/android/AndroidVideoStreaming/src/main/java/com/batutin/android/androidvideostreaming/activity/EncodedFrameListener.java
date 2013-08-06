package com.batutin.android.androidvideostreaming.activity;

/**
 * Created by Andrew Batutin on 8/6/13.
 */
public interface EncodedFrameListener {
    void frameReceived(byte[] outData, int i, int length);
}
