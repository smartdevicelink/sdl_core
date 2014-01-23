package com.ford.syncV4.android.activity.mobilenav;

import android.os.AsyncTask;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SyncProxyTester;

import java.io.IOException;
import java.io.OutputStream;

public class FileStreamingLogic {
    private StaticFileReader staticFileReader;
    private OutputStream outputStream;
    private ServicePreviewFragmentInterface context;
    private Integer fileResID;

    public FileStreamingLogic(ServicePreviewFragmentInterface mobileNavPreviewFragment) {
        context = mobileNavPreviewFragment;
    }

    public OutputStream getOutputStream(){
        return outputStream;
    }

    public void setOutputStream(OutputStream stream){
        this.outputStream = stream;
    }

    public Integer getFileResID() {
        return fileResID;
    }

    public void setFileResID(Integer fileResID) {
        this.fileResID = fileResID;
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
        staticFileReader.execute(fileResID);
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