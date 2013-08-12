package com.batutin.android.androidvideostreaming.media;

import android.media.CamcorderProfile;
import android.media.MediaCodec;
import android.media.MediaFormat;
import android.view.Surface;

import com.batutin.android.androidvideostreaming.activity.ALog;
import com.batutin.android.androidvideostreaming.activity.EncodedFrameListener;
import com.batutin.android.androidvideostreaming.activity.ParameterSetsListener;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.PipedInputStream;
import java.nio.ByteBuffer;

public class VideoAvcCoder {

    public static final int FRAME_RATE = 10;
    public EncodedFrameListener frameListener;
    public ParameterSetsListener parameterSetsListener;
    private PipedInputStream reader;
    private boolean stop = false;
    private MediaEncoder mediaEncoder;
    private MediaDecoder mediaDecoder;
    private Surface surface;


    public VideoAvcCoder(Surface surface, PipedInputStream reader) throws IllegalStateException {
        this.surface = surface;
        this.reader = reader;
        MediaFormat mediaFormat = createEncoderParamets();
        mediaDecoder = new MediaDecoder();
        mediaEncoder = new MediaEncoder();
        mediaEncoder.configureMediaEncoder(mediaFormat);
        start();
    }

    private MediaFormat createEncoderParamets() {
        CamcorderProfile camcorderProfile = CamcorderProfileUtils.getFirstCameraCamcorderProfile(CamcorderProfile.QUALITY_LOW);
        int colorFormat = ColorFormatUtils.selectFirstVideoAvcColorFormat();
        MediaFormat mediaFormat = MediaFormatUtils.createVideoAvcEncoderMediaFormat(camcorderProfile, colorFormat, FRAME_RATE);
        return mediaFormat;
    }

    public void start() {
        try {
            mediaEncoder.start();
        } catch (IllegalStateException exp) {
            ALog.e(exp.getMessage());
        }
    }

    public void stop() throws IOException {
        try {
            this.stop = true;
            mediaEncoder.stop();
            mediaDecoder.stop();
        } catch (IllegalStateException exp) {
            ALog.e(exp.getMessage());
        }
    }

    public void doEncodeDecodeVideoFromBuffer() {

        final int TIMEOUT_USEC = 10000;
        ByteBuffer[] encoderInputBuffers = mediaEncoder.getEncoder().getInputBuffers();
        ByteBuffer[] encoderOutputBuffers = mediaEncoder.getEncoder().getOutputBuffers();
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


        // Loop until the output side is done.
        boolean inputDone = false;
        boolean encoderDone = false;
        boolean outputDone = false;
        while (!outputDone) {
            ALog.v("loop");

            // If we're not done submitting frames, generate a new one and submit it.  By
            // doing this on every loop we're working to ensure that the encoder always has
            // work to do.
            //
            // We don't really want a timeout here, but sometimes there's a delay opening
            // the encoder device, so a short timeout can keep us from spinning hard.
            if (!inputDone) {
                int inputBufIndex = mediaEncoder.getEncoder().dequeueInputBuffer(TIMEOUT_USEC);
                ALog.v("inputBufIndex=" + inputBufIndex);
                if (inputBufIndex >= 0) {
                    long ptsUsec = computePresentationTime(generateIndex);
                    if (stop == true) {
                        // Send an empty frame with the end-of-stream flag set.  If we set EOS
                        // on a frame with data, that frame data will be ignored, and the
                        // output will be short one frame.
                        mediaEncoder.getEncoder().queueInputBuffer(inputBufIndex, 0, 0, ptsUsec,
                                MediaCodec.BUFFER_FLAG_END_OF_STREAM);
                        inputDone = true;
                        ALog.i("sent input EOS (with zero-length frame)");
                    } else {
                        //frameData = new byte[camcorderProfile.videoFrameWidth*camcorderProfile.videoFrameHeight];
                        //new Random().nextBytes(frameData);
                        ByteBuffer inputBuf = encoderInputBuffers[inputBufIndex];
                        inputBuf.clear();
                        //frameData = new byte[camcorderProfile.videoFrameWidth * camcorderProfile.videoFrameHeight * 3 / 2];
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
                        while (res != -1 && bb.size() < mediaEncoder.getMediaFormat().getInteger(MediaFormat.KEY_WIDTH) * mediaEncoder.getMediaFormat().getInteger(MediaFormat.KEY_HEIGHT) * 3 / 2);

                        try {
                            bb.flush();
                        } catch (IOException e) {
                            ALog.e(e.getMessage());
                        }
                        byte[] fd = bb.toByteArray();

                        // The size of a frame of video data, in the formats we handle, is stride*sliceHeight
                        // for Y, and (stride/2)*(sliceHeight/2) for each of the Cb and Cr channels.  Application
                        // of algebra and assuming that stride==width and sliceHeight==height yields:

                        inputBuf.put(fd, 0, mediaEncoder.getMediaFormat().getInteger(MediaFormat.KEY_WIDTH) * mediaEncoder.getMediaFormat().getInteger(MediaFormat.KEY_HEIGHT) * 3 / 2);

                        mediaEncoder.getEncoder().queueInputBuffer(inputBufIndex, 0, mediaEncoder.getMediaFormat().getInteger(MediaFormat.KEY_WIDTH) * mediaEncoder.getMediaFormat().getInteger(MediaFormat.KEY_HEIGHT) * 3 / 2, ptsUsec, 0);
                        ALog.v("submitted frame " + generateIndex + " to enc");
                    }
                    generateIndex++;
                } else {
                    // either all in use, or we timed out during initial setup
                    ALog.i("input buffer not available");
                }
            }

            // Check for output from the encoder.  If there's no output yet, we either need to
            // provide more input, or we need to wait for the encoder to work its magic.  We
            // can't actually tell which is the case, so if we can't get an output buffer right
            // away we loop around and see if it wants more input.
            //
            // Once we get EOS from the encoder, we don't need to do this anymore.
            if (!encoderDone) {
                int encoderStatus = mediaEncoder.getEncoder().dequeueOutputBuffer(info, TIMEOUT_USEC);
                if (encoderStatus == MediaCodec.INFO_TRY_AGAIN_LATER) {
                    // no output available yet
                    ALog.i("no output from encoder available");
                } else if (encoderStatus == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                    // not expected for an encoder
                    encoderOutputBuffers = mediaEncoder.getEncoder().getOutputBuffers();
                    ALog.i("encoder output buffers changed");
                } else if (encoderStatus == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                    // not expected for an encoder
                    MediaFormat newFormat = mediaEncoder.getEncoder().getOutputFormat();
                    ALog.i("encoder output format changed: " + newFormat);
                } else if (encoderStatus < 0) {
                    ALog.e("unexpected result from encoder.dequeueOutputBuffer: " + encoderStatus);
                } else { // encoderStatus >= 0
                    ByteBuffer encodedData = encoderOutputBuffers[encoderStatus];
                    if (encodedData == null) {
                        ALog.i("encoderOutputBuffer " + encoderStatus + " was null");
                    }

                    // It's usually necessary to adjust the ByteBuffer values to match BufferInfo.
                    encodedData.position(info.offset);
                    encodedData.limit(info.offset + info.size);

                    encodedSize += info.size;

                    if ((info.flags & MediaCodec.BUFFER_FLAG_CODEC_CONFIG) != 0) {
                        // Codec config info.  Only expected on first packet.  One way to
                        // handle this is to manually stuff the data into the MediaFormat
                        // and pass that to configure().  We do that here to exercise the API.

                        MediaFormat format = MediaFormatUtils.createVideoAvcDecoderMediaFormat(mediaEncoder.getMediaFormat().getInteger(MediaFormat.KEY_WIDTH), mediaEncoder.getMediaFormat().getInteger(MediaFormat.KEY_HEIGHT), encodedData);
                        mediaDecoder.configureMediaDecoder(format,surface);
                        mediaDecoder.start();
                        decoderInputBuffers = mediaDecoder.getDecoder().getInputBuffers();
                        decoderOutputBuffers = mediaDecoder.getDecoder().getOutputBuffers();
                        decoderConfigured = true;
                        ALog.i("decoder configured (" + info.size + " bytes)");
                    } else {
                        // Get a decoder input buffer, blocking until it's available.

                        int inputBufIndex = mediaDecoder.getDecoder().dequeueInputBuffer(-1);
                        ByteBuffer inputBuf = decoderInputBuffers[inputBufIndex];
                        inputBuf.clear();
                        inputBuf.put(encodedData);
                        mediaDecoder.getDecoder().queueInputBuffer(inputBufIndex, 0, info.size,
                                info.presentationTimeUs, info.flags);

                        encoderDone = (info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0;
                        ALog.v("passed " + info.size + " bytes to decoder"
                                + (encoderDone ? " (EOS)" : ""));
                    }

                    mediaEncoder.getEncoder().releaseOutputBuffer(encoderStatus, false);
                }
            }

            // Check for output from the decoder.  We want to do this on every loop to avoid
            // the possibility of stalling the pipeline.  We use a short timeout to avoid
            // burning CPU if the decoder is hard at work but the next frame isn't quite ready.
            //
            // If we're decoding to a Surface, we'll get notified here as usual but the
            // ByteBuffer references will be null.  The data is sent to Surface instead.
            if (decoderConfigured) {
                int decoderStatus = mediaDecoder.getDecoder().dequeueOutputBuffer(info, TIMEOUT_USEC);
                if (decoderStatus == MediaCodec.INFO_TRY_AGAIN_LATER) {
                    // no output available yet
                    ALog.i("no output from decoder available");
                } else if (decoderStatus == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                    // The storage associated with the direct ByteBuffer may already be unmapped,
                    // so attempting to access data through the old output buffer array could
                    // lead to a native crash.
                    ALog.i("decoder output buffers changed");
                    decoderOutputBuffers = mediaDecoder.getDecoder().getOutputBuffers();
                } else if (decoderStatus == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                    // this happens before the first frame is returned
                    decoderOutputFormat = mediaDecoder.getDecoder().getOutputFormat();
                    ALog.i("decoder output format changed: " +
                            decoderOutputFormat);
                } else if (decoderStatus < 0) {
                    ALog.e("unexpected result from deocder.dequeueOutputBuffer: " + decoderStatus);
                } else {  // decoderStatus >= 0

                    ALog.v("surface decoder given buffer " + decoderStatus +
                            " (size=" + info.size + ")");
                    rawSize += info.size;
                    if ((info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                        ALog.i("output EOS");
                        outputDone = true;
                    }
                    // As soon as we call releaseOutputBuffer, the buffer will be forwarded
                    // to SurfaceTexture to convert to a texture.  The API doesn't guarantee
                    // that the texture will be available before the call returns, so we
                    // need to wait for the onFrameAvailable callback to fire.
                    mediaDecoder.getDecoder().releaseOutputBuffer(decoderStatus, true);
                }
            }
        }

        ALog.i("decoded " + checkIndex + " frames at "
                + mediaEncoder.getMediaFormat().getInteger(MediaFormat.KEY_WIDTH) + "x" + mediaEncoder.getMediaFormat().getInteger(MediaFormat.KEY_HEIGHT) + ": raw=" + rawSize + ", enc=" + encodedSize);

    }

    /**
     * Generates the presentation time for frame N, in microseconds.
     */
    private long computePresentationTime(int frameIndex) {
        return frameIndex * 1000000 / mediaEncoder.getMediaFormat().getInteger(MediaFormat.KEY_FRAME_RATE);
    }
}