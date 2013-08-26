package com.batutin.android.androidvideostreaming.activity;

import android.app.Activity;
import android.graphics.Bitmap;
import android.media.CamcorderProfile;
import android.os.Bundle;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.RelativeLayout;

import com.batutin.android.androidvideostreaming.R;
import com.batutin.android.androidvideostreaming.colorspace.ColorSpaceUtils;
import com.batutin.android.androidvideostreaming.media.CamcorderProfileUtils;
import com.batutin.android.androidvideostreaming.media.VideoAvcCoder;
import com.batutin.android.androidvideostreaming.utils.ALog;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class DecodeActivity extends Activity implements SurfaceHolder.Callback {


    private PlayerThread mPlayer = null;
    private CamcorderProfile mCamcorderProfile;
    private BitmapGeneratorThread mBitmapGenerator;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        configureLogger();
        mCamcorderProfile = CamcorderProfileUtils.getFirstCameraCamcorderProfile(CamcorderProfile.QUALITY_LOW);
        setContentView(R.layout.decode_activity);
        SurfaceView sv = (SurfaceView) findViewById(R.id.surfaceView);
        sv.getHolder().addCallback(this);
    }

    private void configureLogger() {
        ALog.setTag("DecodeActivity");
        ALog.setLevel(ALog.Level.V);
    }

    private void defaultExceptionHandler(Thread paramThread, Throwable paramThrowable) {
        String logMessage = String.format("Thread %d Message %s", paramThread.getId(), paramThrowable.getMessage());
        ALog.e(logMessage);
        if (mPlayer != null) {
            try {
                mPlayer.videoAvcCoder.stop();
            } catch (IllegalStateException e) {
                ALog.e(e.getMessage());
                e.printStackTrace();
            }
            mPlayer.interrupt();
            mBitmapGenerator.interrupt();
        }
    }

    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

        configureVideoCoding(holder);
        ALog.d("surfaceChanged" + mPlayer.toString());
    }

    private void configureVideoCoding(SurfaceHolder holder) {
        PipedInputStream pipedReader = new PipedInputStream();
        PipedOutputStream pipedWriter = new PipedOutputStream();

            mBitmapGenerator = new BitmapGeneratorThread(pipedWriter);


            mPlayer = new PlayerThread(holder.getSurface(), pipedReader);


        try {
            pipedWriter.connect(pipedReader);
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            ALog.i(mBitmapGenerator.getState().name());
            if (mBitmapGenerator.getState().equals(Thread.State.NEW)){
                mBitmapGenerator.start();
            }
            ALog.i(mPlayer.getState().name());
            if (mPlayer.getState().equals(Thread.State.NEW)){
                mPlayer.start();
            }
        } catch (IllegalThreadStateException e) {
            ALog.e(e.getMessage());
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        ALog.d("surfaceDestroyed" + mPlayer.toString());
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (mPlayer != null) {
            endStream(null);
            ALog.d("onPause" + mPlayer.toString());
        }
    }

    public void endStream(View v) {
        mBitmapGenerator.shouldStop();
        mPlayer.videoAvcCoder.shouldStop();

    }

    private byte[] createTestByteArray() {
        RelativeLayout view = (RelativeLayout) findViewById(R.id.testLayout);
        view.buildDrawingCache();
        Bitmap bitmap = Bitmap.createBitmap(view.getDrawingCache(), 0, 0, mCamcorderProfile.videoFrameWidth, mCamcorderProfile.videoFrameHeight);
        view.destroyDrawingCache();
        byte[] data = ColorSpaceUtils.getNV21(mCamcorderProfile.videoFrameWidth, mCamcorderProfile.videoFrameHeight, bitmap);
        return data;
    }

    public class BitmapGeneratorThread extends Thread {

        private final PipedOutputStream pipedOutputStream;
        private boolean stop = false;

        public BitmapGeneratorThread(PipedOutputStream pipedOutputStream) {

            this.pipedOutputStream = pipedOutputStream;

            this.setUncaughtExceptionHandler(new UncaughtExceptionHandler() {
                @Override
                public void uncaughtException(Thread thread, Throwable ex) {
                    defaultExceptionHandler(thread, ex);
                }
            });
        }

        public synchronized void shouldStop() {
            this.stop = true;
        }

        public synchronized PipedOutputStream getPipedOutputStream() {
            return pipedOutputStream;
        }

        @Override
        public void run() {
            try {
                while (!stop) {
                    pipedOutputStream.write(createTestByteArray());
                    pipedOutputStream.flush();
                }
                pipedOutputStream.close();
                mPlayer.videoAvcCoder.shouldStop();
            } catch (Exception e) {
                ALog.e(e.getMessage());
            }
        }
    }

    public class PlayerThread extends Thread {

        public VideoAvcCoder videoAvcCoder;

        public PlayerThread(Surface surface, PipedInputStream pipedReader) {

            this.setUncaughtExceptionHandler(new UncaughtExceptionHandler() {
                @Override
                public void uncaughtException(Thread thread, Throwable ex) {
                    defaultExceptionHandler(thread, ex);
                }
            });
            videoAvcCoder = new VideoAvcCoder(surface, pipedReader);
        }

        @Override
        public String toString() {
            String message = videoAvcCoder.toString();
            return super.toString() + message;
        }

        @Override
        public void run() {
            videoAvcCoder.doEncodeDecodeVideoFromBuffer();
        }

    }


}