package com.batutin.android.androidvideostreaming.activity;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.hardware.Camera;
import android.media.CamcorderProfile;
import android.media.MediaCodec;
import android.media.MediaExtractor;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.RelativeLayout;

import com.batutin.android.androidvideostreaming.R;
import com.batutin.android.androidvideostreaming.media.AvcEncoder;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

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
        ALog.setTag("EncodeDecodeTest");
        ALog.setLevel(ALog.Level.D);
        Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
            @Override
            public void uncaughtException(Thread paramThread, Throwable paramThrowable) {
                String logMessage = String.format("Thread %d Message %s", paramThread.getId(), paramThrowable.getMessage());
                ALog.e(logMessage);
                if (mPlayer != null) {
                    try {
                        mPlayer.av.stop();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    mPlayer.interrupt();
                    bt.interrupt();

                }
            }
        });
        setContentView(R.layout.decode_activity);

        RelativeLayout view = (RelativeLayout)findViewById(R.id.testLayout);
        //view.setDrawingCacheEnabled(true);
        SurfaceView sv = (SurfaceView) findViewById(R.id.surfaceView);
        sv.getHolder().addCallback(this);

    }

    private byte[] createTestByteArray() {
        camcorderProfile = getCamcorderProfile();
        //Bitmap b = BitmapFactory.decodeResource(getResources(),
                //R.drawable.lena);


        RelativeLayout view = (RelativeLayout)findViewById(R.id.testLayout);
        view.buildDrawingCache();
        Bitmap bitmap = Bitmap.createBitmap(view.getDrawingCache(), 0,0 , camcorderProfile.videoFrameWidth, camcorderProfile.videoFrameHeight);

        view.destroyDrawingCache();

        /*View v1 = findViewById(R.id.imageView);
        v1.setDrawingCacheEnabled(true);
        Bitmap bitmap = Bitmap.createBitmap(v1.getDrawingCache(), 0,0 , camcorderProfile.videoFrameWidth, camcorderProfile.videoFrameHeight);
        v1.setDrawingCacheEnabled(false);*/


        byte[] data = getNV21(camcorderProfile.videoFrameWidth, camcorderProfile.videoFrameHeight,bitmap);
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

            if (bt == null) {
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
                mPlayer.av.stop();
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
            this.setUncaughtExceptionHandler(new UncaughtExceptionHandler() {
                @Override
                public void uncaughtException(Thread thread, Throwable ex) {
                    String logMessage = String.format("Thread %d Message %s", thread.getId(), ex.getMessage());
                    ALog.e(logMessage);
                    if (mPlayer != null) {
                        try {
                            mPlayer.av.stop();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        mPlayer.interrupt();
                        bt.interrupt();

                    }
                }
            });
        }



        @Override
        public void run() {
            try {


                int i = 0;
                while (true) {

                    pw.write(createTestByteArray());

                    pw.flush();

                    i++;
                }
            } catch (Exception e) {
                Log.e(TAG, e.toString());
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
            this.setUncaughtExceptionHandler(new UncaughtExceptionHandler() {
                @Override
                public void uncaughtException(Thread thread, Throwable ex) {
                    String logMessage = String.format("Thread %d Message %s", thread.getId(), ex.getMessage());
                    ALog.e(logMessage);
                    if (mPlayer != null) {
                        try {
                            mPlayer.av.stop();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        mPlayer.interrupt();
                        bt.interrupt();

                    }
                }
            });

            av = new AvcEncoder(surface, pipedReader);
        }

        @Override
        public void run() {

            av.doEncodeDecodeVideoFromBuffer();
        }


    }


}