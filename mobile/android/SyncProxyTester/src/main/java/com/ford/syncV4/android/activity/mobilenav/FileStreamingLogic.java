package com.ford.syncV4.android.activity.mobilenav;

import android.os.AsyncTask;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SyncProxyTester;

import java.io.IOException;
import java.io.OutputStream;

public class FileStreamingLogic {
    private StaticFileReader staticFileReader;
    private OutputStream outputStream;
    private MobileNavPreviewFragment context;

    public FileStreamingLogic(MobileNavPreviewFragment mobileNavPreviewFragment) {
        context = mobileNavPreviewFragment;
    }

    public OutputStream getOutputStream(){
        return outputStream;
    }

    public void setOutputStream(OutputStream stream){
        this.outputStream = stream;
    }

    public void cancelStreaming() {
        if (staticFileReader != null) {
            staticFileReader.cancel(true);
        }
    }

    public void startFileStreaming() {
        if (staticFileReader == null || staticFileReader.getStatus() == AsyncTask.Status.FINISHED){
            createStaticFileReader();
        }
        staticFileReader.execute(R.raw.faq_welcome_orientation);
    }

    public void createStaticFileReader() {
        staticFileReader = new StaticFileReader(context.getActivity(), new DataReaderListener() {
            @Override
            public void onStartReading() {
                context.dataStreamingStarted();
            }

            @Override
            public void onDataReceived(final byte[] data) {
                if (outputStream != null && data != null){
                    try {
                        outputStream.write(data);
                    } catch (IOException e) {
                       cancelStreaming();
                       SyncProxyTester tester = (SyncProxyTester) context.getActivity();
                       tester.logError(e);
                    }
                }
            }

            @Override
            public void onCancelReading() {
                context.dataStreamingStopped();
            }

            @Override
            public void onEndReading() {
                context.dataStreamingStopped();
            }
        });
    }
}