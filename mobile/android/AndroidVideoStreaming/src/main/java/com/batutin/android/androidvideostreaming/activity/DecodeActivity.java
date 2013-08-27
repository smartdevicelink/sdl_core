package com.batutin.android.androidvideostreaming.activity;

import android.app.Activity;
import android.graphics.Bitmap;
import android.media.CamcorderProfile;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.RelativeLayout;

import com.batutin.android.androidvideostreaming.R;
import com.batutin.android.androidvideostreaming.colorspace.ColorSpaceUtils;
import com.batutin.android.androidvideostreaming.media.CamcorderProfileUtils;
import com.batutin.android.androidvideostreaming.media.VideoAvcCoder;
import com.batutin.android.androidvideostreaming.media.VideoAvcCoderDataStreamListener;
import com.batutin.android.androidvideostreaming.utils.ALog;
import com.batutin.android.androidvideostreaming.videopreview.BitmapGeneratorThread;
import com.batutin.android.androidvideostreaming.videopreview.PlayerThread;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.nio.ByteBuffer;

public class DecodeActivity extends Activity implements SurfaceHolder.Callback, VideoAvcCoderDataStreamListener {


    private PlayerThread mPlayer;
    private CamcorderProfile mCamcorderProfile;
    private BitmapGeneratorThread mBitmapGenerator;

    public PlayerThread getPlayer() {
        return mPlayer;
    }

    public CamcorderProfile getCamcorderProfile() {
        return mCamcorderProfile;
    }

    public BitmapGeneratorThread getBitmapGenerator() {
        return mBitmapGenerator;
    }

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
        mBitmapGenerator = new BitmapGeneratorThread(this, pipedWriter);
        mPlayer = new PlayerThread(this, holder.getSurface(), pipedReader);
        try {
            pipedWriter.connect(pipedReader);
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            ALog.i(mBitmapGenerator.getState().name());
            if (mBitmapGenerator.getState().equals(Thread.State.NEW)) {
                mBitmapGenerator.start();
            }
            ALog.i(mPlayer.getState().name());
            if (mPlayer.getState().equals(Thread.State.NEW)) {
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

    public byte[] createTestByteArray() {
        RelativeLayout view = (RelativeLayout) findViewById(R.id.testLayout);
        view.buildDrawingCache();
        Bitmap bitmap = Bitmap.createBitmap(view.getDrawingCache(), 0, 0, mCamcorderProfile.videoFrameWidth, mCamcorderProfile.videoFrameHeight);
        view.destroyDrawingCache();
        byte[] data = ColorSpaceUtils.getNV21(mCamcorderProfile.videoFrameWidth, mCamcorderProfile.videoFrameHeight, bitmap);
        return data;
    }


    @Override
    public void dataEncodingShouldStart(VideoAvcCoder videoAvcCoder) {

    }

    @Override
    public void dataEncodingStarted(VideoAvcCoder videoAvcCoder) {

    }

    @Override
    public void frameShouldBeEncoded(VideoAvcCoder videoAvcCoder, byte[] frame) {

    }

    @Override
    public void settingsDataReceived(VideoAvcCoder videoAvcCoder, ByteBuffer settingsData) {

    }

    @Override
    public void frameWasEncoded(VideoAvcCoder videoAvcCoder, ByteBuffer encodedFrame) {

    }

    @Override
    public void dataEncodingShouldStop(VideoAvcCoder videoAvcCoder) {

    }

    @Override
    public void dataEncodingStopped(VideoAvcCoder videoAvcCoder) {

    }

    @Override
    public void dataDecodingShouldStart(VideoAvcCoder videoAvcCoder) {

    }

    @Override
    public void dataDecodingStarted(VideoAvcCoder videoAvcCoder) {

    }

    @Override
    public void frameShouldBeDecoded(VideoAvcCoder videoAvcCoder, ByteBuffer frame) {

    }

    @Override
    public void dataDecodingShouldStop(VideoAvcCoder videoAvcCoder) {

    }

    @Override
    public void dataDecodingStopped(VideoAvcCoder videoAvcCoder) {

    }
}