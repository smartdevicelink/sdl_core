package com.batutin.android.androidvideostreaming.media;

import java.nio.ByteBuffer;

/**
 * Created by Andrew Batutin on 8/14/13.
 */
public interface VideoAvcCoderDataStreamListener {

    public void dataEncodingShouldStart(VideoAvcCoder videoAvcCoder);

    public void dataEncodingStarted(VideoAvcCoder videoAvcCoder);

    public void frameShouldBeEncoded(VideoAvcCoder videoAvcCoder, byte[] frame);

    public void settingsDataReceived(VideoAvcCoder videoAvcCoder, ByteBuffer settingsData);

    public void frameWasEncoded(VideoAvcCoder videoAvcCoder, ByteBuffer encodedFrame);

    public void dataEncodingShouldStop(VideoAvcCoder videoAvcCoder);

    public void dataEncodingStopped(VideoAvcCoder videoAvcCoder);

    public void dataDecodingShouldStart(VideoAvcCoder videoAvcCoder);

    public void dataDecodingStarted(VideoAvcCoder videoAvcCoder);

    public void frameShouldBeDecoded(VideoAvcCoder videoAvcCoder, ByteBuffer frame);

    public void dataDecodingShouldStop(VideoAvcCoder videoAvcCoder);

    public void dataDecodingStopped(VideoAvcCoder videoAvcCoder);

}
