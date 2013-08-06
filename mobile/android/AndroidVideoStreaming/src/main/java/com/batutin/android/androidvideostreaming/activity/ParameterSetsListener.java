package com.batutin.android.androidvideostreaming.activity;

/**
 * Created by Andrew Batutin on 8/6/13.
 */
public interface ParameterSetsListener {
    void avcParametersSetsEstablished(byte[] sps, byte[] pps);
}
