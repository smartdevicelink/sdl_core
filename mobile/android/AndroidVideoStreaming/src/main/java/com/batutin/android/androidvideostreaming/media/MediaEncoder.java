package com.batutin.android.androidvideostreaming.media;

import android.media.MediaCodec;
import android.media.MediaFormat;

import com.batutin.android.androidvideostreaming.activity.ALog;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.PipedInputStream;
import java.nio.ByteBuffer;

/**
 * Created by Andrew Batutin on 8/9/13.
 */
public class MediaEncoder extends AbstractMediaCoder implements MediaCoderState {

    public static final int FRAME_RATE = 10;

    public MediaEncoder() {
        codec = createMediaEncoder();
    }

    public MediaCodec getEncoder() {
        return codec;
    }

    private MediaCodec createMediaEncoder() {
        ALog.d("Start codec creation");
        MediaCodec codec = createMediaCodec();
        ALog.d("End codec creation");
        return codec;
    }

    @Override
    protected MediaCodec createMediaCodec() {
        MediaCodec codec = MediaCodec.createEncoderByType(MIME_TYPE);
        return codec;
    }

    @Override
    public void start() throws IllegalStateException {
        if (isRunning == false) {
            ALog.d("Encoder is going to start");
        } else {
            ALog.d("Encoder is already started");
        }
        super.start();
        if (isRunning == true) {
            ALog.d("Encoder is started");
        } else {
            ALog.d("Encoder is stopped");
        }
    }

    @Override
    public void stop() throws IllegalStateException {
        if (isRunning == true) {
            ALog.d("Encoder is going to stop");
        } else {
            ALog.d("Encoder is already stopped");
        }
        super.stop();
        if (isRunning == false) {
            ALog.d("Encoder is stopped");
        } else {
            ALog.d("Encoder is running");
        }
    }

    public void configureMediaEncoder(MediaFormat format) {
        ALog.d("Start encoder configure");
        mediaFormat = format;
        codec.configure(getMediaFormat(), null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
        isConfigured = true;
        ALog.d("End encoder configure");
    }

    public byte[] getDataToEncode(PipedInputStream reader) {
        ByteArrayOutputStream bb = new ByteArrayOutputStream();
        int res = 0;
        do {
            try {
                res = reader.read();
                if (res != -1) {
                    bb.write(res);
                }
            } catch (IOException e) {
                ALog.e(e.getMessage());
            }
        }
        while (res != -1 && bb.size() < frameSize());

        try {
            bb.flush();
        } catch (IOException e) {
            ALog.e(e.getMessage());
        }
        return bb.toByteArray();
    }

    // The size of a frame of video data, in the formats we handle, is stride*sliceHeight
    // for Y, and (stride/2)*(sliceHeight/2) for each of the Cb and Cr channels.  Application
    // of algebra and assuming that stride==width and sliceHeight==height yields:
    public int frameSize() {
        return getMediaFormat().getInteger(MediaFormat.KEY_WIDTH) * getMediaFormat().getInteger(MediaFormat.KEY_HEIGHT) * 3 / 2;
    }

    public void enqueueFrame(int inputBufIndex, long presentationTimeUs, PipedInputStream reader) {
        ByteBuffer encoderInputBuffer = getEncoder().getInputBuffers()[inputBufIndex];
        encoderInputBuffer.clear();
        byte[] dataToEncode = getDataToEncode(reader);
        encoderInputBuffer.put(dataToEncode, 0, dataToEncode.length);
        getEncoder().queueInputBuffer(inputBufIndex, 0, frameSize(), presentationTimeUs, 0);
    }

    // Send an empty frame with the end-of-stream flag set.  If we set EOS
    // on a frame with data, that frame data will be ignored, and the
    // output will be short one frame.
    public void enqueueEndOfStreamFrame(int inputBufIndex, long presentationTimeUs) {
        getEncoder().queueInputBuffer(inputBufIndex, 0, 0, presentationTimeUs,
                MediaCodec.BUFFER_FLAG_END_OF_STREAM);
    }
}
