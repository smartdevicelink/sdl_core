package com.batutin.android.androidvideostreaming.activity;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;

import com.batutin.android.androidvideostreaming.R;
import com.batutin.android.androidvideostreaming.reader.AssetsReader;
import com.batutin.android.androidvideostreaming.utils.ALog;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.InterruptedIOException;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class VideoStreamingActivity extends Activity {

    private VideoStreamingThread mVideoStream;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.video_streaming);
        configureLogger();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.video_streaming, menu);
        return true;
    }

    public void startStreaming(View v) {
        streamVideoOverTCP();
    }

    private void streamVideoOverTCP() {
        if (mVideoStream == null) {
            AssetsReader reader = new AssetsReader(this);
            InputStream stream = reader.readFileFromAssets("test_video.mp4");
            mVideoStream = new VideoStreamingThread(stream);
            mVideoStream.start();
        }
    }

    private void defaultExceptionHandler(Thread paramThread, Throwable paramThrowable) {
        String logMessage = String.format("Thread %d Message %s", paramThread.getId(), paramThrowable.getMessage());
        ALog.e(logMessage);
    }

    private void configureLogger() {
        ALog.setTag("DecodeActivity");
        ALog.setLevel(ALog.Level.V);
    }

    public class VideoStreamingThread extends Thread {
        private InputStream mStream;

        public VideoStreamingThread(InputStream stream) {
            mStream = stream;
            this.setUncaughtExceptionHandler(new UncaughtExceptionHandler() {
                @Override
                public void uncaughtException(Thread thread, Throwable ex) {
                    defaultExceptionHandler(thread, ex);
                }
            });

        }

        @Override
        public void run() {

        }

        private void runTcpServer() {
            ServerSocket ss = null;
            try {
                ss = new ServerSocket(80);
                //accept connections
                Socket s = ss.accept();
                BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
                BufferedWriter out = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));
                //receive a message
                String incomingMsg = in.readLine() + System.getProperty("line.separator");
                ALog.i("received: " + incomingMsg);
                //send a message
                String outgoingMsg = "goodbye from port " + 80 + System.getProperty("line.separator");
                out.write(outgoingMsg);
                out.flush();
                ALog.i("sent: " + outgoingMsg);
                s.close();
            } catch (InterruptedIOException e) {
                //if timeout occurs
                ALog.e(e.getMessage());

            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                if (ss != null) {
                    try {
                        ss.close();
                    } catch (IOException e) {
                        ALog.e(e.getMessage());
                    }
                }
            }
        }

    }

}
