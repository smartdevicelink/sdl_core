package com.batutin.android.androidvideostreaming.activity;

import android.hardware.Camera;
import android.media.CamcorderProfile;
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaCodecList;
import android.media.MediaFormat;
import android.util.Log;
import android.view.Surface;

import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

public class AvcEncoder {

    public static final String MIME_TYPE = "video/avc";
    private static final String TAG = "EncodeDecodeTest";
    // movie length, in frames
    private static final int NUM_FRAMES = 30;               // two seconds of video
    private static final boolean VERBOSE = false;           // lots of logging
    private static final boolean DEBUG_SAVE_FILE = true;   // save copy of encoded movie
    private static final String DEBUG_FILE_NAME_BASE = "/sdcard/test.";
    public EncodedFrameListener frameListener;
    public ParameterSetsListener parameterSetsListener;
    private MediaCodec mediaCodec;
    private byte[] sps;
    private byte[] pps;
    private MediaCodecInfo codecInfo;
    private CamcorderProfile camcorderProfile;
    private MediaFormat mediaFormat;
    private int colorFormat;
    private byte[] frameData;
    private Surface surface;
    private static final int FRAME_RATE = 15;               // 15fps

    public AvcEncoder() {
        try {
            mediaCodec = MediaCodec.createEncoderByType(MIME_TYPE);
            codecInfo = selectCodec(MIME_TYPE);
            camcorderProfile = getCamcorderProfileHigh();
            mediaFormat = MediaFormat.createVideoFormat("video/avc", camcorderProfile.videoFrameWidth, camcorderProfile.videoFrameHeight);
            mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, camcorderProfile.videoBitRate);
            mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, camcorderProfile.videoFrameRate);
            colorFormat = selectColorFormat(codecInfo, MIME_TYPE);
            mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, colorFormat);
            mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 10);
            mediaCodec.configure(mediaFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
            mediaCodec.start();
        } catch (IllegalStateException e) {
            e.printStackTrace();
        } catch (NullPointerException e) {
            Log.e(this.getClass().getName(), e.getLocalizedMessage());
        }
    }

    private static CamcorderProfile getCamcorderProfileHigh() throws NullPointerException {
        int nCamera = Camera.getNumberOfCameras();
        CamcorderProfile profile = null;
        for (int cameraId = 0; cameraId < nCamera; cameraId++) {
            profile = CamcorderProfile.get(cameraId, CamcorderProfile.QUALITY_LOW);
            break;
        }
        if (profile == null) {
            throw new NullPointerException("CamcorderProfile is null");
        }
        return profile;
    }

    /**
     * Returns the first codec capable of encoding the specified MIME type, or null if no
     * match was found.
     */
    private static MediaCodecInfo selectCodec(String mimeType) {
        int numCodecs = MediaCodecList.getCodecCount();
        for (int i = 0; i < numCodecs; i++) {
            MediaCodecInfo codecInfo = MediaCodecList.getCodecInfoAt(i);

            if (!codecInfo.isEncoder()) {
                continue;
            }

            String[] types = codecInfo.getSupportedTypes();
            for (int j = 0; j < types.length; j++) {
                if (types[j].equalsIgnoreCase(mimeType)) {
                    return codecInfo;
                }
            }
        }
        return null;
    }

    private static int selectColorFormat(MediaCodecInfo codecInfo, String mimeType) throws NullPointerException {
        MediaCodecInfo.CodecCapabilities capabilities = codecInfo.getCapabilitiesForType(mimeType);
        if (capabilities.colorFormats == null || capabilities.colorFormats.length == 0) {
            throw new NullPointerException("Unable to get color formats");
        }
        for (int i = 0; i < capabilities.colorFormats.length; i++) {
            int colorFormat = capabilities.colorFormats[i];
            return colorFormat;
        }
        return 0;   // not reached
    }

    /**
     * Generates the presentation time for frame N, in microseconds.
     */
    private static long computePresentationTime(int frameIndex) {
        return 132 + frameIndex * 1000000 / FRAME_RATE;
    }

    public void close() throws IOException {
        mediaCodec.stop();
        mediaCodec.release();
    }

    public void offerEncoder(byte[] input) {
        try {
            ByteBuffer[] inputBuffers = mediaCodec.getInputBuffers();
            ByteBuffer[] outputBuffers = mediaCodec.getOutputBuffers();
            int inputBufferIndex = mediaCodec.dequeueInputBuffer(-1);
            if (inputBufferIndex >= 0) {
                ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
                inputBuffer.clear();
                inputBuffer.put(input);
                mediaCodec.queueInputBuffer(inputBufferIndex, 0, input.length, 0, 0);
            }

            MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
            int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, -1);
            while (outputBufferIndex >= 0) {
                ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];
                byte[] outData = new byte[bufferInfo.size];
                outputBuffer.get(outData);
                if (sps != null && pps != null) {
                    ByteBuffer frameBuffer = ByteBuffer.wrap(outData);
                    frameBuffer.putInt(bufferInfo.size - 4);
                    frameListener.frameReceived(outData, 0, outData.length);
                } else {
                    ByteBuffer spsPpsBuffer = ByteBuffer.wrap(outData);
                    if (spsPpsBuffer.getInt() == 0x00000001) {
                        System.out.println("parsing sps/pps");
                    } else {
                        System.out.println("something is amiss?");
                    }
                    int ppsIndex = 0;
                    while (!(spsPpsBuffer.get() == 0x00 && spsPpsBuffer.get() == 0x00 && spsPpsBuffer.get() == 0x00 && spsPpsBuffer.get() == 0x01)) {

                    }
                    ppsIndex = spsPpsBuffer.position();
                    sps = new byte[ppsIndex - 8];
                    System.arraycopy(outData, 4, sps, 0, sps.length);
                    pps = new byte[outData.length - ppsIndex];
                    System.arraycopy(outData, ppsIndex, pps, 0, pps.length);
                    if (null != parameterSetsListener) {
                        parameterSetsListener.avcParametersSetsEstablished(sps, pps);
                    }
                }
                mediaCodec.releaseOutputBuffer(outputBufferIndex, false);
                outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
            }
        } catch (Throwable t) {
            t.printStackTrace();
        }

    }

    private void doEncodeDecodeVideoFromBuffer(MediaCodec encoder, int encoderColorFormat,
                                               MediaCodec decoder) {

        long startMs = System.currentTimeMillis();

        final int TIMEOUT_USEC = 10000;
        ByteBuffer[] encoderInputBuffers = encoder.getInputBuffers();
        ByteBuffer[] encoderOutputBuffers = encoder.getOutputBuffers();
        ByteBuffer[] decoderInputBuffers = null;
        ByteBuffer[] decoderOutputBuffers = null;
        MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();
        MediaFormat decoderOutputFormat = null;
        int generateIndex = 0;
        int checkIndex = 0;
        int badFrames = 0;
        boolean decoderConfigured = false;

        // Just out of curiosity.
        long rawSize = 0;
        long encodedSize = 0;

        // Save a copy to disk.  Useful for debugging the test.  Note this is a raw elementary
        // stream, not a .mp4 file, so not all players will know what to do with it.
        FileOutputStream outputStream = null;
        if (DEBUG_SAVE_FILE) {
            String fileName = DEBUG_FILE_NAME_BASE + camcorderProfile.videoFrameWidth + "x" + camcorderProfile.videoFrameHeight + ".mp4";
            try {
                outputStream = new FileOutputStream(fileName);
                Log.d(TAG, "encoded output will be saved as " + fileName);
            } catch (IOException ioe) {
                Log.w(TAG, "Unable to create debug output file " + fileName);
                throw new RuntimeException(ioe);
            }
        }

        // Loop until the output side is done.
        boolean inputDone = false;
        boolean encoderDone = false;
        boolean outputDone = false;
        while (!outputDone) {
            if (VERBOSE) Log.d(TAG, "loop");

            // If we're not done submitting frames, generate a new one and submit it.  By
            // doing this on every loop we're working to ensure that the encoder always has
            // work to do.
            //
            // We don't really want a timeout here, but sometimes there's a delay opening
            // the encoder device, so a short timeout can keep us from spinning hard.
            if (!inputDone) {
                int inputBufIndex = encoder.dequeueInputBuffer(TIMEOUT_USEC);
                if (VERBOSE) Log.d(TAG, "inputBufIndex=" + inputBufIndex);
                if (inputBufIndex >= 0) {
                    long ptsUsec = computePresentationTime(generateIndex);
                    if (generateIndex == NUM_FRAMES) {
                        // Send an empty frame with the end-of-stream flag set.  If we set EOS
                        // on a frame with data, that frame data will be ignored, and the
                        // output will be short one frame.
                        encoder.queueInputBuffer(inputBufIndex, 0, 0, ptsUsec,
                                MediaCodec.BUFFER_FLAG_END_OF_STREAM);
                        inputDone = true;
                        if (VERBOSE) Log.d(TAG, "sent input EOS (with zero-length frame)");
                    } else {


                        ByteBuffer inputBuf = encoderInputBuffers[inputBufIndex];

                        inputBuf.clear();
                        inputBuf.put(frameData);

                        encoder.queueInputBuffer(inputBufIndex, 0, frameData.length, ptsUsec, 0);
                        if (VERBOSE) Log.d(TAG, "submitted frame " + generateIndex + " to enc");
                    }
                    generateIndex++;
                } else {
                    // either all in use, or we timed out during initial setup
                    if (VERBOSE) Log.d(TAG, "input buffer not available");
                }
            }

            // Check for output from the encoder.  If there's no output yet, we either need to
            // provide more input, or we need to wait for the encoder to work its magic.  We
            // can't actually tell which is the case, so if we can't get an output buffer right
            // away we loop around and see if it wants more input.
            //
            // Once we get EOS from the encoder, we don't need to do this anymore.
            if (!encoderDone) {
                int encoderStatus = encoder.dequeueOutputBuffer(info, TIMEOUT_USEC);
                if (encoderStatus == MediaCodec.INFO_TRY_AGAIN_LATER) {
                    // no output available yet
                    if (VERBOSE) Log.d(TAG, "no output from encoder available");
                } else if (encoderStatus == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                    // not expected for an encoder
                    encoderOutputBuffers = encoder.getOutputBuffers();
                    if (VERBOSE) Log.d(TAG, "encoder output buffers changed");
                } else if (encoderStatus == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                    // not expected for an encoder
                    MediaFormat newFormat = encoder.getOutputFormat();
                    if (VERBOSE) Log.d(TAG, "encoder output format changed: " + newFormat);
                } else if (encoderStatus < 0) {
                    Log.e(TAG, "unexpected result from encoder.dequeueOutputBuffer: " + encoderStatus);
                } else { // encoderStatus >= 0
                    ByteBuffer encodedData = encoderOutputBuffers[encoderStatus];
                    if (encodedData == null) {
                        Log.e(TAG, ("encoderOutputBuffer " + encoderStatus + " was null");
                    }

                    // It's usually necessary to adjust the ByteBuffer values to match BufferInfo.
                    encodedData.position(info.offset);
                    encodedData.limit(info.offset + info.size);

                    encodedSize += info.size;
                    if (outputStream != null) {
                        byte[] data = new byte[info.size];
                        encodedData.get(data);
                        encodedData.position(info.offset);
                        try {
                            outputStream.write(data);
                        } catch (IOException ioe) {
                            Log.w(TAG, "failed writing debug data to file");
                            throw new RuntimeException(ioe);
                        }
                    }
                    if ((info.flags & MediaCodec.BUFFER_FLAG_CODEC_CONFIG) != 0) {
                        // Codec config info.  Only expected on first packet.  One way to
                        // handle this is to manually stuff the data into the MediaFormat
                        // and pass that to configure().  We do that here to exercise the API.

                        MediaFormat format =
                                MediaFormat.createVideoFormat(MIME_TYPE, camcorderProfile.videoFrameWidth, camcorderProfile.videoFrameHeight);
                        format.setByteBuffer("csd-0", encodedData);
                        decoder.configure(format, surface,
                                null, 0);
                        decoder.start();
                        decoderInputBuffers = decoder.getInputBuffers();
                        decoderOutputBuffers = decoder.getOutputBuffers();
                        decoderConfigured = true;
                        if (VERBOSE) Log.d(TAG, "decoder configured (" + info.size + " bytes)");
                    } else {
                        // Get a decoder input buffer, blocking until it's available.

                        int inputBufIndex = decoder.dequeueInputBuffer(-1);
                        ByteBuffer inputBuf = decoderInputBuffers[inputBufIndex];
                        inputBuf.clear();
                        inputBuf.put(encodedData);
                        decoder.queueInputBuffer(inputBufIndex, 0, info.size,
                                info.presentationTimeUs, info.flags);

                        encoderDone = (info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0;
                        if (VERBOSE) Log.d(TAG, "passed " + info.size + " bytes to decoder"
                                + (encoderDone ? " (EOS)" : ""));
                    }

                    encoder.releaseOutputBuffer(encoderStatus, false);
                }
            }

            // Check for output from the decoder.  We want to do this on every loop to avoid
            // the possibility of stalling the pipeline.  We use a short timeout to avoid
            // burning CPU if the decoder is hard at work but the next frame isn't quite ready.
            //
            // If we're decoding to a Surface, we'll get notified here as usual but the
            // ByteBuffer references will be null.  The data is sent to Surface instead.
            if (decoderConfigured) {
                int decoderStatus = decoder.dequeueOutputBuffer(info, TIMEOUT_USEC);
                if (decoderStatus == MediaCodec.INFO_TRY_AGAIN_LATER) {
                    // no output available yet
                    if (VERBOSE) Log.d(TAG, "no output from decoder available");
                } else if (decoderStatus == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                    // The storage associated with the direct ByteBuffer may already be unmapped,
                    // so attempting to access data through the old output buffer array could
                    // lead to a native crash.
                    if (VERBOSE) Log.d(TAG, "decoder output buffers changed");
                    decoderOutputBuffers = decoder.getOutputBuffers();
                } else if (decoderStatus == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                    // this happens before the first frame is returned
                    decoderOutputFormat = decoder.getOutputFormat();
                    if (VERBOSE) Log.d(TAG, "decoder output format changed: " +
                            decoderOutputFormat);
                } else if (decoderStatus < 0) {
                    Log.e(TAG, "unexpected result from deocder.dequeueOutputBuffer: " + decoderStatus);
                } else {  // decoderStatus >= 0

                    if (VERBOSE) Log.d(TAG, "surface decoder given buffer " + decoderStatus +
                            " (size=" + info.size + ")");
                    rawSize += info.size;
                    if ((info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                        if (VERBOSE) Log.d(TAG, "output EOS");
                        outputDone = true;
                    }


                    // We use a very simple clock to keep the video FPS, or the video
                    // playback will be too fast
                    while (info.presentationTimeUs / 1000 > System.currentTimeMillis() - startMs) {
                        try {
                            this.wait(10);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                            break;
                        }
                    }
                    // As soon as we call releaseOutputBuffer, the buffer will be forwarded
                    // to SurfaceTexture to convert to a texture.  The API doesn't guarantee
                    // that the texture will be available before the call returns, so we
                    // need to wait for the onFrameAvailable callback to fire.
                    decoder.releaseOutputBuffer(decoderStatus, true);


                }
            }
        }

        if (VERBOSE) Log.d(TAG, "decoded " + checkIndex + " frames at "
                + camcorderProfile.videoFrameWidth + "x" + camcorderProfile.videoFrameHeight + ": raw=" + rawSize + ", enc=" + encodedSize);
        if (outputStream != null) {
            try {
                outputStream.close();
            } catch (IOException ioe) {
                Log.w(TAG, "failed closing debug file");
                throw new RuntimeException(ioe);
            }
        }
    }
}