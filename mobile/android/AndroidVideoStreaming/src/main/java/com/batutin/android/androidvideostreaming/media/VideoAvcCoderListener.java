package com.batutin.android.androidvideostreaming.media;

/**
 * Created by Andrew Batutin on 8/24/13.
 */
public interface VideoAvcCoderListener {

    public void coderStarted(VideoAvcCoder listener);

    public void errorOnCoderStart(VideoAvcCoder listener, String errorDescription);

    public void coderShouldStop(VideoAvcCoder videoAvcCoder);

    public void coderStopped(VideoAvcCoder videoAvcCoder);

    public void errorOnCoderStop(VideoAvcCoder videoAvcCoder, String errorMessage);
}
