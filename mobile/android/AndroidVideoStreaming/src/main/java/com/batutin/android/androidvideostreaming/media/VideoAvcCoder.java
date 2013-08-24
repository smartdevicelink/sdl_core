package com.batutin.android.androidvideostreaming.media;

import android.media.CamcorderProfile;
import android.media.MediaCodec;
import android.media.MediaFormat;
import android.view.Surface;

import com.batutin.android.androidvideostreaming.utils.ALog;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

public class VideoAvcCoder {

    private VideoAvcCoderListener videoAvcCoderListener;
    private PresentationTimeCalc presentationTimeCalc;
    private InputStream reader;
    private boolean stop = false;
    private MediaEncoder mediaEncoder;
    private MediaDecoder mediaDecoder;
    private Surface surface;

    private VideoAvcCoder() {
    }

    public VideoAvcCoder(Surface surface, InputStream reader, int quality, VideoAvcCoderListener videoAvcCoderListener) {
        initCoder(surface, reader, quality, videoAvcCoderListener);
    }

    private void initCoder(Surface surface, InputStream reader, int quality, VideoAvcCoderListener videoAvcCoderListener) {
        this.videoAvcCoderListener = checkListenerIsNull(videoAvcCoderListener);
        this.surface = surface;
        this.reader = reader;
        MediaFormat mediaFormat = createEncoderParameters(quality);
        mediaDecoder = new MediaDecoder();
        mediaEncoder = new MediaEncoder();
        mediaEncoder.configureMediaEncoder(mediaFormat);
        presentationTimeCalc = new PresentationTimeCalc(MediaEncoder.FRAME_RATE);
    }

    public static VideoAvcCoder createLowQualityVideoAvcCoder(Surface surface, InputStream reader, VideoAvcCoderListener videoAvcCoderListener) {
        return new VideoAvcCoder(surface, reader, CamcorderProfile.QUALITY_LOW, videoAvcCoderListener);
    }

    public static VideoAvcCoder createHighQualityVideoAvcCoder(Surface surface, InputStream reader, VideoAvcCoderListener videoAvcCoderListener) {
        return new VideoAvcCoder(surface, reader, CamcorderProfile.QUALITY_HIGH, videoAvcCoderListener);
    }

    public VideoAvcCoderListener getVideoAvcCoderListener() {
        return videoAvcCoderListener;
    }

    protected VideoAvcCoderListener checkListenerIsNull(VideoAvcCoderListener videoAvcCoderListener) throws IllegalArgumentException {
        if (videoAvcCoderListener == null) {
            throw new IllegalArgumentException("VideoAvcCoderListener should not be null");
        }
        return videoAvcCoderListener;
    }

    public InputStream getReader() {
        return reader;
    }

    public MediaEncoder getMediaEncoder() {
        return mediaEncoder;
    }

    public MediaDecoder getMediaDecoder() {
        return mediaDecoder;
    }

    protected MediaFormat createEncoderParameters(int quality) {
        CamcorderProfile camcorderProfile = CamcorderProfileUtils.getFirstCameraCamcorderProfile(quality);
        int colorFormat = ColorFormatUtils.selectFirstVideoAvcColorFormat();
        MediaFormat mediaFormat = MediaFormatUtils.createVideoAvcEncoderMediaFormat(camcorderProfile, colorFormat, MediaEncoder.FRAME_RATE);
        return mediaFormat;
    }

    public synchronized void start() {
        try {
            mediaEncoder.start();
            videoAvcCoderListener.coderStarted(this);
        } catch (IllegalStateException exp) {
            ALog.e(exp.getMessage());
            videoAvcCoderListener.errorOnCoderStart(this, exp.getMessage());
        }
    }

    public synchronized void stop() {
        this.stop = true;
        videoAvcCoderListener.coderShouldStop(this);
    }

    public synchronized void forceStop() {
        stopCoding();
    }

    private void stopCoding() {
        try {
            reader.close();
            mediaEncoder.stop();
            mediaDecoder.stop();
            videoAvcCoderListener.coderStopped(this);
        } catch (IllegalStateException exp) {
            ALog.e(exp.getMessage());
            videoAvcCoderListener.errorOnCoderStop(this, "Something wrong with coder " + exp.getMessage());
        } catch (IOException exp) {
            videoAvcCoderListener.errorOnCoderStop(this, "Something wrong with coder stream " + exp.getMessage());
            ALog.e(exp.getMessage());
        }catch (NullPointerException exp){
            videoAvcCoderListener.errorOnCoderStop(this, "some element is null " + exp.getMessage());
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
                int inputBufIndex = mediaEncoder.getEncoder().dequeueInputBuffer(-1);
                ALog.v("inputBufIndex=" + inputBufIndex);
                if (inputBufIndex >= 0) {
                    long presentationTimeUs = presentationTimeCalc.computePresentationTime(generateIndex);
                    if (stop == true) {

                        mediaEncoder.enqueueEndOfStreamFrame(inputBufIndex, presentationTimeUs);
                        inputDone = true;
                        ALog.i("sent input EOS (with zero-length frame)");
                    } else {

                        mediaEncoder.enqueueFrame(inputBufIndex, presentationTimeUs, reader);
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
                int encoderStatus = mediaEncoder.getEncoder().dequeueOutputBuffer(info, -1);
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
                    encodedSize = matchBufferInfo(info, encodedSize, encodedData);
                    if ((info.flags & MediaCodec.BUFFER_FLAG_CODEC_CONFIG) != 0) {
                        // Codec config info.  Only expected on first packet.  One way to
                        // handle this is to manually stuff the data into the MediaFormat
                        // and pass that to configure().  We do that here to exercise the API.
                        startDecoder(encodedData);
                        ALog.i("decoder configured (" + info.size + " bytes) and started");
                    } else {
                        // Get a decoder input buffer, blocking until it's available.
                        mediaDecoder.queueEncodedData(info, encodedData, this);
                        encoderDone = isEncoderDone(info);
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
            if (mediaDecoder.isConfigured()) {
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
        stopCoding();
    }

    private long matchBufferInfo(MediaCodec.BufferInfo info, long encodedSize, ByteBuffer encodedData) {
        encodedData.position(info.offset);
        encodedData.limit(info.offset + info.size);
        encodedSize += info.size;
        return encodedSize;
    }

    private boolean isEncoderDone(MediaCodec.BufferInfo info) {
        boolean encoderDone;
        encoderDone = (info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0;
        return encoderDone;
    }

    private void startDecoder(ByteBuffer csd0) {
        MediaFormat format = MediaFormatUtils.createVideoAvcDecoderMediaFormat(mediaEncoder.getMediaFormat().getInteger(MediaFormat.KEY_WIDTH), mediaEncoder.getMediaFormat().getInteger(MediaFormat.KEY_HEIGHT), csd0);
        mediaDecoder.configureMediaDecoder(format, surface);
        mediaDecoder.start();
    }

    @Override
    public String toString() {
        String message = " " + mediaDecoder.toString() + " " + mediaEncoder.toString();
        return super.toString() + message;
    }
}