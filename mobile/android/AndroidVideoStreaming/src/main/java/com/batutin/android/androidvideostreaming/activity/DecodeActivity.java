package com.batutin.android.androidvideostreaming.activity;

import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.hardware.Camera;
import android.media.CamcorderProfile;
import android.media.MediaCodec;
import android.media.MediaExtractor;
import android.media.MediaFormat;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import com.batutin.android.androidvideostreaming.R;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.nio.ByteBuffer;

public class DecodeActivity extends Activity implements SurfaceHolder.Callback {

    // parameters for the encoder
    private static final String MIME_TYPE = "video/avc";    // H.264 Advanced Video Coding
    private static final int FRAME_RATE = 15;               // 15fps
    private static final int IFRAME_INTERVAL = 10;          // 10 seconds between I-frames
    private static final String SAMPLE = Environment.getExternalStorageDirectory() + "/video.mp4";
    private static final String TAG = "EncodeDecodeTest";
    private PlayerThread mPlayer = null;
    private CamcorderProfile camcorderProfile;
    private byte[] res;
    private BitmapThread bt;

    private static CamcorderProfile getCamcorderProfile() throws NullPointerException {
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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.decode_activity);


        SurfaceView sv = (SurfaceView) findViewById(R.id.surfaceView);
        sv.getHolder().addCallback(this);

    }

    private byte[] createTestByteArray() {
        camcorderProfile = getCamcorderProfile();
        Bitmap b = BitmapFactory.decodeResource(getResources(),
                R.drawable.lena);

        byte[] data = getNV21(camcorderProfile.videoFrameWidth, camcorderProfile.videoFrameHeight, b);
        return data;
    }

    byte[] getNV21(int inputWidth, int inputHeight, Bitmap scaled) {

        int[] argb = new int[inputWidth * inputHeight];

        scaled.getPixels(argb, 0, inputWidth, 0, 0, inputWidth, inputHeight);

        byte[] yuv = new byte[inputWidth * inputHeight * 3 / 2];
        encodeYUV420SP(yuv, argb, inputWidth, inputHeight);

        scaled.recycle();

        return yuv;
    }

    void encodeYUV420SP(byte[] yuv420sp, int[] argb, int width, int height) {
        final int frameSize = width * height;

        int yIndex = 0;
        int uvIndex = frameSize;

        int a, R, G, B, Y, U, V;
        int index = 0;
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {

                a = (argb[index] & 0xff000000) >> 24; // a is not used obviously
                R = (argb[index] & 0xff0000) >> 16;
                G = (argb[index] & 0xff00) >> 8;
                B = (argb[index] & 0xff) >> 0;

                // well known RGB to YUV algorithm
                Y = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
                U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
                V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;

                // NV21 has a plane of Y and interleaved planes of VU each sampled by a factor of 2
                //    meaning for every 4 Y pixels there are 1 V and 1 U.  Note the sampling is every other
                //    pixel AND every other scanline.
                yuv420sp[yIndex++] = (byte) ((Y < 0) ? 0 : ((Y > 255) ? 255 : Y));
                if (j % 2 == 0 && index % 2 == 0) {
                    yuv420sp[uvIndex++] = (byte) ((V < 0) ? 0 : ((V > 255) ? 255 : V));
                    yuv420sp[uvIndex++] = (byte) ((U < 0) ? 0 : ((U > 255) ? 255 : U));
                }

                index++;
            }
        }
    }

    private Bitmap getBitmapFromView(View view) {
        Bitmap returnedBitmap = Bitmap.createBitmap(camcorderProfile.videoFrameWidth, camcorderProfile.videoFrameHeight, Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(returnedBitmap);
        Drawable bgDrawable = view.getBackground();
        if (bgDrawable != null)
            bgDrawable.draw(canvas);
        else
            canvas.drawColor(Color.WHITE);
        view.draw(canvas);
        return returnedBitmap;
    }

    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        if (mPlayer == null) {

            PipedInputStream pipedReader = new PipedInputStream();

            PipedOutputStream pipedWriter = new PipedOutputStream();

            if (res == null) {
                res = createTestByteArray();
                bt = new BitmapThread(pipedWriter, res);
                mPlayer = new PlayerThread(holder.getSurface(), res, pipedReader);
            }


            // Connect the writer with reader

            try {
                pipedWriter.connect(pipedReader);
            } catch (IOException e) {
                e.printStackTrace();
            }


            bt.start();


            mPlayer.start();
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        if (mPlayer != null) {
            try {
                mPlayer.av.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            mPlayer.interrupt();
            bt.interrupt();

        }
    }

    public class BitmapThread extends Thread {
        private final PipedOutputStream pw;
        private byte[] bytes;

        public BitmapThread(PipedOutputStream pw, byte[] res) {
            bytes = res;
            this.pw = pw;

        }

        @Override
        public void run() {
            try {

                int i = 0;
                while (i < 30) {

                    pw.write(bytes);

                    pw.flush();
                    i++;
                }
            } catch (Exception e) {
                Log.e(TAG, e.getLocalizedMessage());
                System.out.println(" PipeThread Exception: " + e);
            }
        }
    }

    public class PlayerThread extends Thread {
        private MediaExtractor extractor;
        private MediaCodec decoder;
        private Surface surface;
        public AvcEncoder av;

        public PlayerThread(Surface surface, byte[] res, PipedInputStream pipedReader) {
            this.surface = surface;

            // Create writer and reader instances


            //byte[] frame = new byte[1000*600];

            //AssetsReader r = new AssetsReader(DecodeActivity.this);
            //InputStream is = r.readFileFromAssets("test_video.yuv");
            //VideoStreaming vs = new VideoStreaming();
            //byte[] b = vs.readTestVideoFileFromStream(is, null);
            av = new AvcEncoder(surface, pipedReader);


        }

        @Override
        public void run() {
            /*try {
                todo();
            } catch (IOException e) {
                e.printStackTrace();
            }*/
            av.doEncodeDecodeVideoFromBuffer(new ThreadPause());
        }

        private void todo() throws IOException {
            extractor = new MediaExtractor();
            AssetFileDescriptor testFd = getResources().openRawResourceFd(R.raw.test_video);
            extractor.setDataSource(testFd.getFileDescriptor(), testFd.getStartOffset(),
                    testFd.getLength());

            for (int i = 0; i < extractor.getTrackCount(); i++) {
                MediaFormat format = extractor.getTrackFormat(i);
                String mime = format.getString(MediaFormat.KEY_MIME);
                if (mime.startsWith("video/")) {
                    extractor.selectTrack(i);
                    decoder = MediaCodec.createDecoderByType(mime);
                    decoder.configure(format, surface, null, 0);
                    break;
                }
            }

            if (decoder == null) {
                Log.e("DecodeActivity", "Can't find video info!");
                return;
            }

            decoder.start();

            ByteBuffer[] inputBuffers = decoder.getInputBuffers();
            ByteBuffer[] outputBuffers = decoder.getOutputBuffers();
            MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();
            boolean isEOS = false;
            long startMs = System.currentTimeMillis();

            while (!Thread.interrupted()) {
                if (!isEOS) {
                    int inIndex = decoder.dequeueInputBuffer(10000);
                    if (inIndex >= 0) {
                        ByteBuffer buffer = inputBuffers[inIndex];

                        int sampleSize = extractor.readSampleData(buffer, 0);
                        StringBuilder b = new StringBuilder();

                        for (int i = 0; i < buffer.remaining(); i++) {
                            char buf = (char) buffer.get(i);
                            b.append(buf);
                        }

                        String r = b.toString();
                        if (sampleSize < 0) {
                            // We shouldn't stop the playback at this point, just pass the EOS
                            // flag to decoder, we will get it again from the
                            // dequeueOutputBuffer
                            Log.d("DecodeActivity", "InputBuffer BUFFER_FLAG_END_OF_STREAM");
                            decoder.queueInputBuffer(inIndex, 0, 0, 0, MediaCodec.BUFFER_FLAG_END_OF_STREAM);
                            isEOS = true;
                        } else {
                            decoder.queueInputBuffer(inIndex, 0, sampleSize, extractor.getSampleTime(), 0);
                            extractor.advance();
                        }
                    }
                }

                int outIndex = decoder.dequeueOutputBuffer(info, 10000);
                switch (outIndex) {
                    case MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED:
                        Log.d("DecodeActivity", "INFO_OUTPUT_BUFFERS_CHANGED");
                        outputBuffers = decoder.getOutputBuffers();
                        break;
                    case MediaCodec.INFO_OUTPUT_FORMAT_CHANGED:
                        Log.d("DecodeActivity", "New format " + decoder.getOutputFormat());
                        break;
                    case MediaCodec.INFO_TRY_AGAIN_LATER:
                        Log.d("DecodeActivity", "dequeueOutputBuffer timed out!");
                        break;
                    default:
                        ByteBuffer buffer = outputBuffers[outIndex];
                        Log.v("DecodeActivity", "We can't use this buffer but render it due to the API limit, " + buffer);

                        // We use a very simple clock to keep the video FPS, or the video
                        // playback will be too fast
                        while (info.presentationTimeUs / 1000 > System.currentTimeMillis() - startMs) {
                            try {
                                sleep(10);
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                                break;
                            }
                        }
                        decoder.releaseOutputBuffer(outIndex, true);
                        break;
                }

                // All decoded frames have been rendered, we can stop playing now
                if ((info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                    Log.d("DecodeActivity", "OutputBuffer BUFFER_FLAG_END_OF_STREAM");
                    break;
                }
            }

            decoder.stop();
            decoder.release();
            extractor.release();
        }

        public class ThreadPause {
            public void pause() {
                try {
                    sleep(10);

                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

            }
        }
    }


}