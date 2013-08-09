package com.batutin.android.androidvideostreaming.activity;

import android.app.Activity;
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaCodecList;
import android.media.MediaFormat;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Menu;

import com.batutin.android.androidvideostreaming.R;
import com.batutin.android.androidvideostreaming.media.AvcEncoder;
import com.batutin.android.androidvideostreaming.reader.AssetsReader;
import com.batutin.android.androidvideostreaming.reader.FileStreamReaderListener;
import com.batutin.android.androidvideostreaming.reader.VideoStreaming;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

public class MainActivity extends Activity implements FileStreamReaderListener, EncodedFrameListener, ParameterSetsListener {

    private static final String TAG = "ColorSpaceUtilsTest";
    private static final int WIDTH = 256;
    private static final int HEIGHT = 128;
    private AvcEncoder encoder;
    private ByteArrayOutputStream bb;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        bb = new ByteArrayOutputStream();
        encoder = new AvcEncoder(null, null);
        encoder.frameListener = this;
        encoder.parameterSetsListener = this;
        AssetsReader r = new AssetsReader(this);
        InputStream is = r.readFileFromAssets("test_video.yuv");
        VideoStreaming vs = new VideoStreaming();
        try {
            vs.readTestVideoFileFromStream(is, this);
        } catch (IOException e) {
            e.printStackTrace();
        }




        //this.encodeDecodeVideo();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    private void encodeDecodeVideo() {
        int width = 328, height = 248;
        int bitRate = 1000000;
        int frameRate = 15;
        String mimeType = "video/avc";
        int threshold = 50;
        int maxerror = 50;

        MediaCodec encoder, decoder = null;
        ByteBuffer[] encoderInputBuffers;
        ByteBuffer[] encoderOutputBuffers;
        ByteBuffer[] decoderInputBuffers = null;
        ByteBuffer[] decoderOutputBuffers = null;

        int numCodecs = MediaCodecList.getCodecCount();
        MediaCodecInfo codecInfo = getMediaCodecInfo(mimeType, numCodecs);
        Log.d(TAG, "Found " + codecInfo.getName() + " supporting " + mimeType);
        MediaCodecInfo.CodecCapabilities capabilities = codecInfo.getCapabilitiesForType(mimeType);
        int colorFormat = getCapabilities(capabilities);

        Log.d(TAG, "Using color format " + colorFormat);

        width = calcWidth(codecInfo, width);
        int stride = calcStrideForWidth(width, codecInfo);
        int sliceHeight = calcSliceHeightForHeight(height, codecInfo);
        MediaFormat inputFormat = getMediaFormat(width, height, bitRate, frameRate, mimeType, colorFormat, stride, sliceHeight);
        encoder = startMediaCodec(codecInfo, inputFormat);
        encoderInputBuffers = encoder.getInputBuffers();
        encoderOutputBuffers = encoder.getOutputBuffers();

        int chromaStride = calcChromaStride(stride);
        int frameSize = calcFrameStride(stride, sliceHeight, chromaStride);
        byte[] inputFrame = createInputFrame(width, height, colorFormat, stride, sliceHeight, chromaStride, frameSize);

        // start encoding + decoding
        final long kTimeOutUs = 5000;
        MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();
        boolean sawInputEOS = false;
        boolean sawOutputEOS = false;
        MediaFormat oformat = null;
        int errors = -1;
        int numInputFrames = 0;
        while (!sawOutputEOS && errors < 0) {
            if (!sawInputEOS) {
                int inputBufIndex = encoder.dequeueInputBuffer(kTimeOutUs);

                if (inputBufIndex >= 0) {
                    ByteBuffer dstBuf = encoderInputBuffers[inputBufIndex];

                    int sampleSize = frameSize;
                    long presentationTimeUs = 0;

                    if (numInputFrames >= 10) {
                        Log.d(TAG, "saw input EOS.");
                        sawInputEOS = true;
                        sampleSize = 0;
                    } else {
                        dstBuf.clear();
                        dstBuf.put(inputFrame);
                        presentationTimeUs = numInputFrames * 1000000 / frameRate;
                        numInputFrames++;
                    }

                    encoder.queueInputBuffer(
                            inputBufIndex,
                            0 /* offset */,
                            sampleSize,
                            presentationTimeUs,
                            sawInputEOS ? MediaCodec.BUFFER_FLAG_END_OF_STREAM : 0);
                }
            }

            int res = encoder.dequeueOutputBuffer(info, kTimeOutUs);

            if (res >= 0) {
                int outputBufIndex = res;
                ByteBuffer buf = encoderOutputBuffers[outputBufIndex];

                buf.position(info.offset);
                buf.limit(info.offset + info.size);

                if ((info.flags & MediaCodec.BUFFER_FLAG_CODEC_CONFIG) != 0) {

                    decoder = MediaCodec.createDecoderByType(mimeType);
                    MediaFormat format = MediaFormat.createVideoFormat(mimeType, width, height);
                    format.setByteBuffer("csd-0", buf);
                    decoder.configure(format, null /* surface */, null /* crypto */, 0 /* flags */);
                    decoder.start();
                    decoderInputBuffers = decoder.getInputBuffers();
                    decoderOutputBuffers = decoder.getOutputBuffers();
                } else {
                    int decIndex = decoder.dequeueInputBuffer(-1);
                    decoderInputBuffers[decIndex].clear();
                    decoderInputBuffers[decIndex].put(buf);
                    decoder.queueInputBuffer(decIndex, 0, info.size, info.presentationTimeUs, info.flags);
                }

                encoder.releaseOutputBuffer(outputBufIndex, false /* render */);
            } else if (res == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                encoderOutputBuffers = encoder.getOutputBuffers();

                Log.d(TAG, "encoder output buffers have changed.");
            } else if (res == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                MediaFormat encformat = encoder.getOutputFormat();

                Log.d(TAG, "encoder output format has changed to " + encformat);
            }

            if (decoder == null)
                res = MediaCodec.INFO_TRY_AGAIN_LATER;
            else
                res = decoder.dequeueOutputBuffer(info, kTimeOutUs);

            if (res >= 0) {
                int outputBufIndex = res;
                ByteBuffer buf = decoderOutputBuffers[outputBufIndex];

                buf.position(info.offset);
                buf.limit(info.offset + info.size);

                if (info.size > 0) {
                    //errors = checkFrame(buf, info, oformat, width, height, threshold);
                }

                decoder.releaseOutputBuffer(outputBufIndex, false /* render */);

                if ((info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                    Log.d(TAG, "saw output EOS.");
                    sawOutputEOS = true;
                }
            } else if (res == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                decoderOutputBuffers = decoder.getOutputBuffers();

                Log.d(TAG, "decoder output buffers have changed.");
            } else if (res == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                oformat = decoder.getOutputFormat();

                Log.d(TAG, "decoder output format has changed to " + oformat);
            }

        }

        encoder.stop();
        encoder.release();
        decoder.stop();
        decoder.release();


    }

    private MediaCodec startMediaCodec(MediaCodecInfo codecInfo, MediaFormat inputFormat) {
        MediaCodec encoder = MediaCodec.createByCodecName(codecInfo.getName());
        Log.d(TAG, "Configuring encoder with input format " + inputFormat);
        encoder.configure(inputFormat, null /* surface */, null /* crypto */, MediaCodec.CONFIGURE_FLAG_ENCODE);
        encoder.start();
        return encoder;
    }

    private byte[] createInputFrame(int width, int height, int colorFormat, int stride, int sliceHeight, int chromaStride, int frameSize) {
        byte[] inputFrame = new byte[frameSize];
        if (colorFormat == MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar ||
                colorFormat == MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420PackedPlanar) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int Y = (x + y) & 255;
                    int Cb = 255 * x / width;
                    int Cr = 255 * y / height;
                    inputFrame[y * stride + x] = (byte) Y;
                    inputFrame[stride * sliceHeight + (y / 2) * chromaStride + (x / 2)] = (byte) Cb;
                    inputFrame[stride * sliceHeight + chromaStride * (sliceHeight / 2) + (y / 2) * chromaStride + (x / 2)] = (byte) Cr;
                }
            }
        } else {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int Y = (x + y) & 255;
                    int Cb = 255 * x / width;
                    int Cr = 255 * y / height;
                    inputFrame[y * stride + x] = (byte) Y;
                    inputFrame[stride * sliceHeight + 2 * (y / 2) * chromaStride + 2 * (x / 2)] = (byte) Cb;
                    inputFrame[stride * sliceHeight + 2 * (y / 2) * chromaStride + 2 * (x / 2) + 1] = (byte) Cr;
                }
            }
        }
        return inputFrame;
    }

    private int calcFrameStride(int stride, int sliceHeight, int chromaStride) {
        return stride * sliceHeight + 2 * chromaStride * sliceHeight / 2;
    }

    private int calcChromaStride(int stride) {
        return stride / 2;
    }

    private MediaFormat getMediaFormat(int width, int height, int bitRate, int frameRate, String mimeType, int colorFormat, int stride, int sliceHeight) {
        MediaFormat inputFormat = MediaFormat.createVideoFormat(mimeType, width, height);
        inputFormat.setInteger(MediaFormat.KEY_BIT_RATE, bitRate);
        inputFormat.setInteger(MediaFormat.KEY_FRAME_RATE, frameRate);
        inputFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, colorFormat);
        inputFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 75);
        inputFormat.setInteger("stride", stride);
        inputFormat.setInteger("slice-height", sliceHeight);
        return inputFormat;
    }

    private int calcSliceHeightForHeight(int height, MediaCodecInfo codecInfo) {
        int sliceHeight = height;
        if (codecInfo.getName().startsWith("OMX.Nvidia.")) {
            sliceHeight = (sliceHeight + 15) / 16 * 16;
        }
        return sliceHeight;
    }

    private int calcStrideForWidth(int width, MediaCodecInfo codecInfo) {
        int stride = width;
        if (codecInfo.getName().startsWith("OMX.Nvidia.")) {
            stride = (stride + 15) / 16 * 16;
        }
        return stride;
    }

    private int calcWidth(MediaCodecInfo codecInfo, int width) {
        if (codecInfo.getName().equals("OMX.TI.DUCATI1.VIDEO.H264E")) {
            // This codec doesn't support a width not a multiple of 16,
            // so round down.
            width &= ~15;
        }
        return width;
    }

    private int getCapabilities(MediaCodecInfo.CodecCapabilities capabilities) {
        int colorFormat = 0;
        for (int i = 0; i < capabilities.colorFormats.length && colorFormat == 0; i++) {
            int format = capabilities.colorFormats[i];
            switch (format) {
                case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar:
                case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420PackedPlanar:
                case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar:
                case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420PackedSemiPlanar:
                case MediaCodecInfo.CodecCapabilities.COLOR_TI_FormatYUV420PackedSemiPlanar:
                    colorFormat = format;
                    break;
                default:
                    Log.d(TAG, "Skipping unsupported color format " + format);
                    break;
            }
        }
        return colorFormat;
    }

    private MediaCodecInfo getMediaCodecInfo(String mimeType, int numCodecs) {
        MediaCodecInfo codecInfo = null;
        for (int i = 0; i < numCodecs && codecInfo == null; i++) {
            MediaCodecInfo info = MediaCodecList.getCodecInfoAt(i);
            if (!info.isEncoder()) {
                continue;
            }
            String[] types = info.getSupportedTypes();
            boolean found = false;
            for (int j = 0; j < types.length && !found; j++) {
                if (types[j].equals(mimeType))
                    found = true;
            }
            if (!found)
                continue;
            codecInfo = info;
        }
        return codecInfo;
    }

    @Override
    public void fileReadWillBegin(VideoStreaming source) {

    }

    @Override
    public void chunkIsReadFromFile(VideoStreaming source, byte[] chunk) {

    }

    @Override
    public void fileReadEnded(VideoStreaming source) {
        try {
            bb.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
        byte[] b= bb.toByteArray();

        File sdCard = Environment.getExternalStorageDirectory();
        File dir = new File (sdCard.getAbsolutePath() + "/dir1/dir2");
        dir.mkdirs();
        File file = new File(dir, "filename");

        try {
            FileOutputStream fos = new FileOutputStream(file);
            fos.write(b);
            fos.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    @Override
    public void frameReceived(byte[] outData, int i, int length) {
        try {
            bb.write(outData);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void avcParametersSetsEstablished(byte[] sps, byte[] pps) {

    }
}
