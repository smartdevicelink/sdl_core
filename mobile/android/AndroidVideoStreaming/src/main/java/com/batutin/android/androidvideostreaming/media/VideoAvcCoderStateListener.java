package com.batutin.android.androidvideostreaming.media;

/**
 * Created by Andrew Batutin on 8/14/13.
 */
public interface VideoAvcCoderStateListener {

    public void videoAvcCoderShouldStart(VideoAvcCoder videoAvcCoder);
    public void videoAvcCoderStarted (VideoAvcCoder videoAvcCoder);
    public void videoAvcCoderShouldStop(VideoAvcCoder videoAvcCoder);
    public void videoAvcCoderStopped(VideoAvcCoder videoAvcCoder);

}
