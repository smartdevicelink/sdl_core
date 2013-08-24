package com.batutin.android.androidvideostreaming.media;

/**
 * Created by Andrew Batutin on 8/12/13.
 */
public interface MediaCoderState {

    public void start() throws IllegalStateException;

    public void stop() throws IllegalStateException;
}
