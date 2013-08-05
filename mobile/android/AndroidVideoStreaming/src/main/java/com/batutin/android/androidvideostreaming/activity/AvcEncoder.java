package com.batutin.android.androidvideostreaming.activity;

import android.hardware.Camera;
import android.media.CamcorderProfile;
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaCodecList;
import android.media.MediaFormat;
import android.util.Log;

import java.io.IOException;
import java.nio.ByteBuffer;

public class AvcEncoder {

    public static final String MIME_TYPE = "video/avc";
    //private EncodedFrameListener frameListener;
    private MediaCodec mediaCodec;
    private byte[] sps;
    private byte[] pps;
    private MediaCodecInfo codecInfo;
    //private ParameterSetsListener parameterSetsListener;

    public AvcEncoder() {
        try {
            mediaCodec = MediaCodec.createEncoderByType(MIME_TYPE);
            codecInfo = selectCodec(MIME_TYPE);
            CamcorderProfile p = getCamcorderProfileHigh();
            MediaFormat mediaFormat = MediaFormat.createVideoFormat("video/avc", p.videoFrameWidth, p.videoFrameHeight);
            mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, p.videoBitRate);
            mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, p.videoFrameRate);
            int colorFormat = selectColorFormat(codecInfo, MIME_TYPE);
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
            profile = CamcorderProfile.get(cameraId, CamcorderProfile.QUALITY_HIGH);
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

    private static int selectColorFormat(MediaCodecInfo codecInfo, String mimeType) throws NullPointerException{
        MediaCodecInfo.CodecCapabilities capabilities = codecInfo.getCapabilitiesForType(mimeType);
        if ( capabilities.colorFormats == null || capabilities.colorFormats.length ==0 ){
            throw new NullPointerException("Unable to get color formats");
        }
        for (int i = 0; i < capabilities.colorFormats.length; i++) {
            int colorFormat = capabilities.colorFormats[i];
                return colorFormat;
        }
        return 0;   // not reached
    }

    /**
     * Returns true if this is a color format that this test code understands (i.e. we know how
     * to read and generate frames in this format).
     */
    private static boolean isRecognizedFormat(int colorFormat) {
        switch (colorFormat) {
            // these are the formats we know how to handle for this test
            case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar:
            case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420PackedPlanar:
            case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar:
            case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420PackedSemiPlanar:
            case MediaCodecInfo.CodecCapabilities.COLOR_TI_FormatYUV420PackedSemiPlanar:
                return true;
            default:
                return false;
        }
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
            int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
            while (outputBufferIndex >= 0) {
                ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];
                byte[] outData = new byte[bufferInfo.size];
                outputBuffer.get(outData);
                if (sps != null && pps != null) {
                    ByteBuffer frameBuffer = ByteBuffer.wrap(outData);
                    frameBuffer.putInt(bufferInfo.size - 4);
                    //frameListener.frameReceived(outData, 0, outData.length);
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
                    //if (null != parameterSetsListener) {
                    //    parameterSetsListener.avcParametersSetsEstablished(sps, pps);
                    //}
                }
                mediaCodec.releaseOutputBuffer(outputBufferIndex, false);
                outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
            }
        } catch (Throwable t) {
            t.printStackTrace();
        }

    }
}