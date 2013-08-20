package com.batutin.android.androidvideostreaming.media;

/**
 * Created by Andrew Batutin on 8/12/13.
 */
public interface MediaCoderState {
    void start() throws IllegalStateException;

    void stop() throws IllegalStateException;
}
